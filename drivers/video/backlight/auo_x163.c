/* linux/drivers/video/exynos/auo_x163.c
 *
 * MIPI-DSI based auo_x163 AMOLED lcd 4.65 inch panel driver.
 *
 * Inki Dae, <inki.dae@auo_x163.com>
 * Donghwa Lee, <dh09.lee@auo_x163.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/lcd.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/regulator/consumer.h>
#include <linux/gpio.h>

#include <video/mipi_display.h>
#include <mach/jz_dsim.h>

extern struct regulator_dev *regulator_to_rdev(struct regulator *regulator);
extern int ricoh61x_regulator_set_sleep_mode_power(struct regulator_dev *rdev, int power_on);


#define POWER_IS_ON(pwr)	((pwr) == FB_BLANK_UNBLANK)
#define POWER_IS_OFF(pwr)	((pwr) == FB_BLANK_POWERDOWN)
#define POWER_IS_NRM(pwr)	((pwr) == FB_BLANK_NORMAL)

#define lcd_to_master(a)	(a->dsim_dev->master)
#define lcd_to_master_ops(a)	((lcd_to_master(a))->master_ops)

struct auo_x163_platform_data{
    struct lcd_platform_data *lcd_pdata;
    char *vcc_lcd_1v8_name;
    char *vcc_lcd_3v0_name;
    char *vcc_lcd_blk_name;
    int swire_gpio;
    int swire_active_level;
};

struct auo_x163 {
    struct device *dev;
    unsigned int power;
    unsigned int id;

    struct lcd_device *ld;
    struct backlight_device *bd;

    struct mipi_dsim_lcd_device *dsim_dev;
    struct auo_x163_platform_data *ddi_pd;
    struct mutex lock;
    struct regulator *vcc_lcd_1v8_reg;
    struct regulator *vcc_lcd_3v0_reg;
    struct regulator *vcc_lcd_blk_reg;
    char *vcc_lcd_1v8_name;
    char *vcc_lcd_3v0_name;
    char *vcc_lcd_blk_name;
};


static void auo_x163_brightness_setting(struct auo_x163 *lcd, unsigned long value);

static inline int bl_regulator(struct auo_x163 *lcd, int en)
{
    struct regulator *reg;

    if (!lcd || !lcd->vcc_lcd_blk_reg)
        return 0;

    reg = lcd->vcc_lcd_blk_reg;

    return (en ? regulator_enable(reg) : regulator_disable(reg));
}

static void auo_x163_regulator_enable(struct auo_x163 *lcd)
{
	regulator_enable(lcd->vcc_lcd_3v0_reg);
	regulator_enable(lcd->vcc_lcd_1v8_reg);

#if defined(CONFIG_SLPT) && defined(CONFIG_REGULATOR_RICOH619)
	ricoh61x_regulator_set_sleep_mode_power(regulator_to_rdev(lcd->vcc_lcd_3v0_reg), 1);
	ricoh61x_regulator_set_sleep_mode_power(regulator_to_rdev(lcd->vcc_lcd_1v8_reg), 1);
#endif


	msleep(10);
}

static void auo_x163_regulator_disable(struct auo_x163 *lcd)
{
	int ret = 0;

	/* quick action buttons to lcd display abnormal */
	ret = bl_regulator(lcd, 0);
	if (ret < 0)
		pr_err("auo_x163: disable bl regulator err\n");

	/* next arrival operation mast delay >120ms */
	msleep(120);

	ret = regulator_force_disable(lcd->vcc_lcd_1v8_reg);
	if (ret)
		printk("can't disable auo_x163 vcc_1v8_reg ++++++++++\n");

	ret = regulator_force_disable(lcd->vcc_lcd_3v0_reg);
	if (ret)
		printk("can't disable auo_x163 vcc_3v0_reg ++++++++++\n");

#if defined(CONFIG_SLPT) && defined(CONFIG_REGULATOR_RICOH619)
	ricoh61x_regulator_set_sleep_mode_power(regulator_to_rdev(lcd->vcc_lcd_3v0_reg), 0);
	ricoh61x_regulator_set_sleep_mode_power(regulator_to_rdev(lcd->vcc_lcd_1v8_reg), 0);
#endif

}

static void auo_x163_sleep_in(struct auo_x163 *lcd)
{
	unsigned char data_to_send[] = {0x05, 0x10, 0x00};
	int array_size = ARRAY_SIZE(data_to_send);
	struct dsi_master_ops *ops = lcd_to_master_ops(lcd);
	struct dsi_device *dsi = lcd_to_master(lcd);
	ops->cmd_write(dsi, data_to_send, array_size);
}

static void auo_x163_sleep_out(struct auo_x163 *lcd)
{
	unsigned char data_to_send[] = {0x05, 0x11, 0x00};
	int array_size = ARRAY_SIZE(data_to_send);
	struct dsi_master_ops *ops = lcd_to_master_ops(lcd);
	struct dsi_device *dsi = lcd_to_master(lcd);
	ops->cmd_write(dsi, data_to_send, array_size);
}

static void auo_x163_display_on(struct auo_x163 *lcd)
{
	unsigned char data_to_send[] = {0x05, 0x29, 0x00};
	int array_size = ARRAY_SIZE(data_to_send);
	struct dsi_master_ops *ops = lcd_to_master_ops(lcd);
	struct dsi_device *dsi = lcd_to_master(lcd);
	ops->cmd_write(dsi, data_to_send, array_size);
}

static void auo_x163_display_off(struct auo_x163 *lcd)
{
	unsigned char data_to_send[] = {0x05, 0x28, 0x00};
	int array_size = ARRAY_SIZE(data_to_send);
	struct dsi_master_ops *ops = lcd_to_master_ops(lcd);
	struct dsi_device *dsi = lcd_to_master(lcd);
	ops->cmd_write(dsi, data_to_send, array_size);
}


static int auo_x163_set_power(struct lcd_device *ld, int power)
{
	int ret = 0;
	return ret;
}

static int auo_x163_get_power(struct lcd_device *ld)
{
	struct auo_x163 *lcd = lcd_get_data(ld);

	return lcd->power;
}


static struct lcd_ops auo_x163_lcd_ops = {
	.set_power = auo_x163_set_power,
	.get_power = auo_x163_get_power,
};

/*
 *?????????????????????????????????,????????????????????????????????????,
 *????????????????????????????????????,??????????????????????????????0x05,0x15,0x39,??????0x05,0x15????????????,0x39????????????
 *??????????????????????????????????????????????????????????????????
 *???????????????????????????????????????????????????????????????????????????????????????????????????
 *?????????????????????????????????????????????????????????
*/

unsigned char auo_x163_cmd_list1[] = {
#ifdef CONFIG_AUO_X163_MARK
	0x39, 0x06, 0x00, 0xf0, 0x55, 0xaa, 0x52, 0x08, 0x00,
	0x39, 0x06, 0x00, 0xbd, 0x03, 0x20, 0x14, 0x4b, 0x00,
	0x39, 0x06, 0x00, 0xbe, 0x03, 0x20, 0x14, 0x4b, 0x01,
	0x39, 0x06, 0x00, 0xbf, 0x03, 0x20, 0x14, 0x4b, 0x00,
#else
	0x39, 0x06, 0x00, 0xf0, 0x55, 0xaa, 0x52, 0x88, 0x00,
	0x39, 0x06, 0x00, 0xbd, 0x01, 0x90, 0x14, 0x14, 0x00,
	0x39, 0x06, 0x00, 0xbe, 0x01, 0x90, 0x14, 0x14, 0x01,
	0x39, 0x06, 0x00, 0xbf, 0x01, 0x90, 0x14, 0x14, 0x00,
#endif
	0x39, 0x04, 0x00, 0xbb, 0x07, 0x07, 0x07,
	0x39, 0x02, 0x00, 0xc7, 0x40,
	0x39, 0x02, 0x00, 0x51, 0x64,
	0x39, 0x02, 0x00, 0x53, 0x20,
	0x39, 0x06, 0x00, 0xf0, 0x55, 0xaa, 0x52, 0x08, 0x02,
#ifdef CONFIG_AUO_X163_MARK
	0x15, 0xeb, 0x02,
#endif
	0x39, 0x03, 0x00, 0xfe, 0x08, 0x50,
#ifdef CONFIG_AUO_X163_MARK
	0x39, 0x04, 0x00, 0xe9, 0x00, 0x36,0x38,
#endif
	0x39, 0x04, 0x00, 0xc3, 0xf2, 0x95, 0x04,
	0x39, 0x02, 0x00, 0xca, 0x04,
	0x39, 0x06, 0x00, 0xf0, 0x55, 0xaa, 0x52, 0x08, 0x01,
	0x39, 0x04, 0x00, 0xb0, 0x03, 0x03, 0x03,

	0x39, 0x05, 0x00, 0x2a, 0x00, 0x00, (319 & 0xff00) >> 8, 479 & 0xff,
	0x39, 0x05, 0x00, 0x2b, 0x00, 0x00, (559 & 0xff00) >> 8, 559 & 0xff,

	0x39, 0x04, 0x00, 0xb1, 0x05, 0x05, 0x05,
	0x39, 0x04, 0x00, 0xb2, 0x01, 0x01, 0x01,
	0x39, 0x04, 0x00, 0xb4, 0x07, 0x07, 0x07,
	0x39, 0x04, 0x00, 0xb5, 0x03, 0x03, 0x03,
#ifdef CONFIG_AUO_X163_MARK
	0x39, 0x04, 0x00, 0xb6, 0x55, 0x55, 0x55,
	0x39, 0x04, 0x00, 0xb7, 0x36, 0x36, 0x36,
#else
	0x39, 0x04, 0x00, 0xb6, 0x53, 0x53, 0x53,
	0x39, 0x04, 0x00, 0xb7, 0x33, 0x33, 0x33,
#endif
	0x39, 0x04, 0x00, 0xb8, 0x23, 0x23, 0x23,
	0x39, 0x04, 0x00, 0xb9, 0x03, 0x03, 0x03,
	0x39, 0x04, 0x00, 0xba, 0x03, 0x03, 0x03,
	0x39, 0x04, 0x00, 0xbe, 0x32, 0x30, 0x70,
#ifdef CONFIG_AUO_X163_MARK
	0x39, 0x08, 0x00, 0xcf, 0xff, 0xd4, 0x95, 0xe8, 0x4f, 0x00, 0x04,
#else
	0x39, 0x08, 0x00, 0xcf, 0xff, 0xd4, 0x95, 0xef, 0x4f, 0x00, 0x04,
#endif
	0x39, 0x02, 0x00, 0x35, 0x00,
#ifdef CONFIG_AUO_X163_ROTATION_180
	0x39, 0x02, 0x00, 0x36, 0xc0,
#else
	0x39, 0x02, 0x00, 0x36, 0x00,
#endif
	0x39, 0x02, 0x00, 0xc0, 0x20,

	0x39, 0x07, 0x00, 0xc2, 0x17, 0x17, 0x17, 0x17, 0x17, 0x0b,

//	0x39, 0x05, 0x00, 0x2a, 0x00, 0x00, (479 & 0xff00) >> 8, 319 & 0xff,
//	0x39, 0x05, 0x00, 0x2b, 0x00, 0x00, (319 & 0xff00) >> 8, 319 & 0xff,

//	0x39, 0x01, 0x00, 0x2c,

};

unsigned char auo_x163_cmd_list2[] = {
	0x39, 0x05, 0x00, 0x2a, 0x00, 0x00, (319 & 0xff00) >> 8, 479 & 0xff,
	0x39, 0x05, 0x00, 0x2b, 0x00, 0x00, (319 & 0xff00) >> 8, 319 & 0xff,
//	0x39, 0x05, 0x00, 0x2a, 0x00, 0x00, 0x01, 0xdf,
//	0x39, 0x05, 0x00, 0x2b, 0x00, 0x00, 0x01, 0x3f,

};

#if 1
static void auo_x163_panel_condition_setting(struct auo_x163 *lcd)
{
	int array_size = ARRAY_SIZE(auo_x163_cmd_list1);
	struct dsi_master_ops *ops = lcd_to_master_ops(lcd);
	struct dsi_device *dsi = lcd_to_master(lcd);
	ops->cmd_write(dsi, auo_x163_cmd_list1, array_size);
}
#endif
#if 0
static void auo_x163_panel_condition_setting1(struct auo_x163 *lcd)
{
	int array_size = ARRAY_SIZE(auo_x163_cmd_list2);
	struct dsi_master_ops *ops = lcd_to_master_ops(lcd);
	struct dsi_device *dsi = lcd_to_master(lcd);
	ops->cmd_write(dsi, auo_x163_cmd_list2, array_size);
}
#endif

/*
 * This can enter idle mode and auo_x163_exit_idle will exit idle mode 
 * BUT: they are still not join the system
 * */
static void auo_x163_enter_idle(struct auo_x163 *lcd)
{
        unsigned char auo_x163_cmd_bl[] = {
			//{0x39, 0x02, 0x00, {0x39}},
			0x05, 0x39, 0x00,
        };
		int array_size = ARRAY_SIZE(auo_x163_cmd_bl);
		struct dsi_master_ops *ops = lcd_to_master_ops(lcd);
		struct dsi_device *dsi = lcd_to_master(lcd);
		ops->cmd_write(dsi, auo_x163_cmd_bl, array_size);
}

static void auo_x163_exit_idle(struct auo_x163 *lcd)
{
        unsigned char auo_x163_cmd_bl[] = {
			//{0x39, 0x02, 0x00, {0x38}},
			0x05, 0x38, 0x00,
        };
		int array_size = ARRAY_SIZE(auo_x163_cmd_bl);
		struct dsi_master_ops *ops = lcd_to_master_ops(lcd);
		struct dsi_device *dsi = lcd_to_master(lcd);
		ops->cmd_write(dsi, auo_x163_cmd_bl, array_size);
}

static void auo_x163_brightness_setting(struct auo_x163 *lcd, unsigned long value)
{
        unsigned char auo_x163_cmd_bl[] = {
			0x39, 0x02, 0x00, 0x51, value,
			0x39, 0x02, 0x00, 0x53, 0x20,
        };
		int array_size = ARRAY_SIZE(auo_x163_cmd_bl);
		struct dsi_master_ops *ops = lcd_to_master_ops(lcd);
		struct dsi_device *dsi = lcd_to_master(lcd);
		ops->cmd_write(dsi, auo_x163_cmd_bl, array_size);
        //auo_x163_enter_idle(lcd);
}

static int auo_x163_backlight_update_status(struct backlight_device *bd)
{
        struct auo_x163 *lcd = dev_get_drvdata(&bd->dev);
        int brightness = bd->props.brightness;
        auo_x163_brightness_setting(lcd, brightness);
        return 0;
}

static const struct backlight_ops auo_x163_backlight_ops = {
        .update_status  = auo_x163_backlight_update_status,
};

static void auo_x163_set_sequence(struct mipi_dsim_lcd_device *dsim_dev)
{
    struct auo_x163 *lcd = dev_get_drvdata(&dsim_dev->dev);
    //auo_x163_panel_init(lcd);
    mutex_lock(&lcd->lock);
    if (!lcd->ddi_pd->lcd_pdata->lcd_enabled && lcd->ddi_pd->lcd_pdata->reset) {
        lcd->ddi_pd->lcd_pdata->reset(lcd);
    }


    auo_x163_panel_condition_setting(lcd);

    auo_x163_sleep_out(lcd);

    /* wakeup default brightness */
    auo_x163_brightness_setting(lcd, 0);

    /* Sleep out to DISPLAY ON mast delay > 300ms* */
    msleep(40);
    //auo_x163_display_on(lcd);

    mutex_unlock(&lcd->lock);
    lcd->power = FB_BLANK_UNBLANK;
}

static int auo_x163_ioctl(struct mipi_dsim_lcd_device *dsim_dev, int cmd)
{
    struct auo_x163 *lcd = dev_get_drvdata(&dsim_dev->dev);
    int ret = -1;
    int i = 0;
    if (!lcd) {
        pr_err(" auo_x163_ioctl get drv failed\n");
        return -1;
    }
    mutex_lock(&lcd->lock);
    switch (cmd) {
    case CMD_MIPI_DISPLAY_ON:
        msleep(10);
        /* send DISPLAY ON to power on */
        auo_x163_display_on(lcd);

        /* Wait swire signal active */
        if (lcd->ddi_pd->swire_gpio > 0) {
            while((ret != lcd->ddi_pd->swire_active_level) && (i < 100)) {
                ret = gpio_get_value(lcd->ddi_pd->swire_gpio);
                msleep(10);
                i++;
            }
        }

        /* enable backlight power */
        bl_regulator(lcd, 1);
        break;
    default:
        break;
    }
    mutex_unlock(&lcd->lock);
    return 0;
}

static int auo_x163_regulator_get(struct auo_x163 *lcd)
{
	int err = 0;

	lcd->vcc_lcd_1v8_reg = regulator_get(NULL, lcd->vcc_lcd_1v8_name);
	pr_debug("+++++++++++++++++++++vcc_lcd_1v8_name: %s++++++++++++++\n", lcd->vcc_lcd_1v8_name);
	if (IS_ERR(lcd->vcc_lcd_1v8_reg)) {
		printk("failed to get VCC regulator.");
		err = PTR_ERR(lcd->vcc_lcd_1v8_reg);
		goto return_err;
	}

	lcd->vcc_lcd_3v0_reg = regulator_get(NULL, lcd->vcc_lcd_3v0_name);
	pr_debug("+++++++++++++++++++++vcc_lcd_3v0_name: %s++++++++++++++\n", lcd->vcc_lcd_3v0_name);
	if (IS_ERR(lcd->vcc_lcd_3v0_reg)) {
		printk("failed to get VCC regulator.");
		err = PTR_ERR(lcd->vcc_lcd_3v0_reg);
		regulator_put(lcd->vcc_lcd_1v8_reg);
		goto error_get_vcc_lcd_3v0;
	}

	if (lcd->vcc_lcd_blk_name) {
		lcd->vcc_lcd_blk_reg = regulator_get(NULL, lcd->vcc_lcd_blk_name);
		if (IS_ERR(lcd->vcc_lcd_blk_reg)) {
			printk("failed to get blk regulator.");
			err = PTR_ERR(lcd->vcc_lcd_blk_reg);
			goto error_get_vcc_lcd_blk;
		}
	}

	regulator_enable(lcd->vcc_lcd_1v8_reg);
	regulator_enable(lcd->vcc_lcd_3v0_reg);
	if (lcd->vcc_lcd_blk_reg)
		regulator_enable(lcd->vcc_lcd_blk_reg);

	goto return_err;
error_get_vcc_lcd_blk:
	regulator_put(lcd->vcc_lcd_3v0_reg);
error_get_vcc_lcd_3v0:
	regulator_put(lcd->vcc_lcd_1v8_reg);
return_err:
	return err;
}

static int auo_x163_probe(struct mipi_dsim_lcd_device *dsim_dev)
{
	struct auo_x163 *lcd;
	int err;
        struct backlight_properties props;

        props.type = BACKLIGHT_RAW;
	props.max_brightness = 255;
	lcd = devm_kzalloc(&dsim_dev->dev, sizeof(struct auo_x163), GFP_KERNEL);
	if (!lcd) {
		dev_err(&dsim_dev->dev, "failed to allocate auo_x163 structure.\n");
		return -ENOMEM;
	}

	lcd->dsim_dev = dsim_dev;
	lcd->ddi_pd = (struct  auo_x163_platform_data*)dsim_dev->platform_data;
	lcd->dev = &dsim_dev->dev;

	mutex_init(&lcd->lock);

	lcd->ld = lcd_device_register("auo_x163", lcd->dev, lcd,
								  &auo_x163_lcd_ops);
	if (IS_ERR(lcd->ld)) {
		dev_err(lcd->dev, "failed to register lcd ops.\n");
		return PTR_ERR(lcd->ld);
	}

        lcd->bd = backlight_device_register("pwm-backlight.0", lcd->dev, lcd,
                                &auo_x163_backlight_ops, &props);
	lcd->vcc_lcd_1v8_name = lcd->ddi_pd->vcc_lcd_1v8_name;
	pr_debug("+++++++++++++++++++=== auo vcc_lcd_1v8_name : %s+++++++++++++\n", lcd->vcc_lcd_1v8_name);
	lcd->vcc_lcd_3v0_name = lcd->ddi_pd->vcc_lcd_3v0_name;
	pr_debug("+++++++++++++++++++=== auo vcc_lcd_3v0_name : %s+++++++++++++\n", lcd->vcc_lcd_3v0_name);
	lcd->vcc_lcd_blk_name = lcd->ddi_pd->vcc_lcd_blk_name;

	err = auo_x163_regulator_get(lcd);
	if(err){
		dev_err(&dsim_dev->dev, "failed to get regulator\n");
		return err;
	}

    /* Swire protocol setting pin of Power IC*/
    if (lcd->ddi_pd->swire_gpio > 0) {
        err = gpio_request(lcd->ddi_pd->swire_gpio, "auo_x163_swire");
        if (err < 0) {
            printk(" %s failed to set gpio swire.\n",__func__);
            return err;
        }
        gpio_direction_input(lcd->ddi_pd->swire_gpio);
    }

    dev_set_drvdata(&dsim_dev->dev, lcd);

    dev_dbg(lcd->dev, "probed auo_x163 panel driver.\n");
    return 0;
}

#ifdef CONFIG_PM
static int auo_x163_suspend(struct mipi_dsim_lcd_device *dsim_dev)
{
    struct auo_x163 *lcd = dev_get_drvdata(&dsim_dev->dev);

    mutex_lock(&lcd->lock);
    auo_x163_brightness_setting(lcd, 0);

    auo_x163_display_off(lcd);
    auo_x163_sleep_in(lcd);

    auo_x163_regulator_disable(lcd);

    lcd->ddi_pd->lcd_pdata->lcd_enabled = false;
    mutex_unlock(&lcd->lock);

    return 0;
}

static int auo_x163_resume(struct mipi_dsim_lcd_device *dsim_dev, int mode)
{
	struct auo_x163 *lcd = dev_get_drvdata(&dsim_dev->dev);

	mutex_lock(&lcd->lock);

    if (!lcd->ddi_pd->lcd_pdata->lcd_enabled) {
	auo_x163_regulator_enable(lcd);
    }

    mutex_unlock(&lcd->lock);

    msleep(lcd->ddi_pd->lcd_pdata->power_on_delay);

    return 0;
}

static void auo_x163_power_on(struct mipi_dsim_lcd_device *dsim_dev, int power)
{
	struct auo_x163 *lcd = dev_get_drvdata(&dsim_dev->dev);
	auo_x163_resume(dsim_dev, power);
	/* lcd power on */
	if (lcd->ddi_pd->lcd_pdata->power_on)
		lcd->ddi_pd->lcd_pdata->power_on(lcd->ld, power);
}
#else
#define auo_x163_suspend		NULL
#define auo_x163_resume		NULL
#endif


static struct mipi_dsim_lcd_driver auo_x163_dsim_ddi_driver = {
	.name = "auo_x163-lcd",
	.id = 0,

	.power_on = auo_x163_power_on,
	.set_sequence = auo_x163_set_sequence,
	.ioctl        = auo_x163_ioctl,
	.probe = auo_x163_probe,
	.suspend = auo_x163_suspend,
	.resume = auo_x163_resume,
};

static int auo_x163_init(void)
{
	mipi_dsi_register_lcd_driver(&auo_x163_dsim_ddi_driver);
	return 0;
}

static void auo_x163_exit(void)
{
	return;
}

module_init(auo_x163_init);
module_exit(auo_x163_exit);

MODULE_DESCRIPTION("auo_x163 lcd driver");
MODULE_LICENSE("GPL");
