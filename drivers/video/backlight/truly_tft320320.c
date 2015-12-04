/*
 *  LCD control code for truly
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/lcd.h>
#include <linux/fb.h>
#include <linux/regulator/consumer.h>

struct truly_tft320320_data {
	int lcd_power;
	struct lcd_device *lcd;
	struct lcd_platform_data *ctrl;
};

static int truly_tft320320_set_power(struct lcd_device *lcd, int power)
{
	struct truly_tft320320_data *dev= lcd_get_data(lcd);

	if (!power && dev->lcd_power) {
		dev->ctrl->power_on(lcd, 1);
	} else if (power && !dev->lcd_power) {
		if (dev->ctrl->reset) {
			dev->ctrl->reset(lcd);
		}
		dev->ctrl->power_on(lcd, 0);
	}
	dev->lcd_power = power;
	return 0;
}

static int truly_tft320320_get_power(struct lcd_device *lcd)
{
	struct truly_tft320320_data *dev= lcd_get_data(lcd);

	return dev->lcd_power;
}

static int truly_tft320320_set_mode(struct lcd_device *lcd, struct fb_videomode *mode)
{
	return 0;
}

static struct lcd_ops truly_tft320320_ops = {
	.set_power = truly_tft320320_set_power,
	.get_power = truly_tft320320_get_power,
	.set_mode = truly_tft320320_set_mode,
};

static int __devinit truly_tft320320_probe(struct platform_device *pdev)
{
	int ret;
	struct truly_tft320320_data *dev;

	dev = kzalloc(sizeof(struct truly_tft320320_data), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	dev->ctrl = pdev->dev.platform_data;
	if (dev->ctrl == NULL) {
		dev_info(&pdev->dev, "no platform data!");
		return -EINVAL;
	}

	dev_set_drvdata(&pdev->dev, dev);

	dev->lcd = lcd_device_register("truly_tft320320_slcd", &pdev->dev,
				       dev, &truly_tft320320_ops);
	if (IS_ERR(dev->lcd)) {
		ret = PTR_ERR(dev->lcd);
		dev->lcd = NULL;
		dev_info(&pdev->dev, "lcd device register error: %d\n", ret);
	} else {
		dev_info(&pdev->dev, "lcd device(TRULY TFT320320) register success\n");
	}

	if (dev->ctrl->power_on) {
		dev->ctrl->power_on(dev->lcd, 1);
		dev->lcd_power = FB_BLANK_UNBLANK;
	}


	return 0;
}

static int __devinit truly_tft320320_remove(struct platform_device *pdev)
{
	struct truly_tft320320_data *dev = dev_get_drvdata(&pdev->dev);

	if (dev->lcd_power)
		dev->ctrl->power_on(dev->lcd, 0);

	lcd_device_unregister(dev->lcd);
	dev_set_drvdata(&pdev->dev, NULL);
	kfree(dev);

	return 0;
}

#ifdef CONFIG_PM
static int truly_tft320320_suspend(struct platform_device *pdev,
		pm_message_t state)
{
	return 0;
}

static int truly_tft320320_resume(struct platform_device *pdev)
{
	return 0;
}
#else
#define truly_tft320320_suspend	NULL
#define truly_tft320320_resume	NULL
#endif

static struct platform_driver truly_tft320320_driver = {
	.driver		= {
		.name	= "truly_tft320320_slcd",
		.owner	= THIS_MODULE,
	},
	.probe		= truly_tft320320_probe,
	.remove		= truly_tft320320_remove,
	.suspend	= truly_tft320320_suspend,
	.resume		= truly_tft320320_resume,
};

static int __init truly_tft320320_init(void)
{
	return platform_driver_register(&truly_tft320320_driver);
}
module_init(truly_tft320320_init);

static void __exit truly_tft320320_exit(void)
{
	platform_driver_unregister(&truly_tft320320_driver);
}
module_exit(truly_tft320320_exit);

MODULE_DESCRIPTION("TRULY TFT320320 lcd panel driver");
MODULE_LICENSE("GPL");
