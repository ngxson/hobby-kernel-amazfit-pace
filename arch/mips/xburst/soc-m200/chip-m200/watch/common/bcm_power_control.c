#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <mach/jzmmc.h>
#include <linux/bcm_pm_core.h>
#include <linux/delay.h>
#include <soc/base.h>
#include <linux/fs.h>
#include <linux/wlan_plat.h>
#include <asm/uaccess.h>

#include "board_base.h"

#ifdef CONFIG_BCM_PM_CORE
static void enable_clk32k(void)
{
	jzrtc_enable_clk32k();
}

static void disable_clk32k(void)
{
	jzrtc_disable_clk32k();
}

static struct bcm_power_platform_data bcm_power_platform_data = {
	.wlan_pwr_en = BCM_PWR_EN,
	.clk_enable = enable_clk32k,
	.clk_disable = disable_clk32k,
};

struct platform_device	bcm_power_platform_device = {
	.name = "bcm_power",
	.id = -1,
	.num_resources = 0,
	.dev = {
		.platform_data = &bcm_power_platform_data,
	},
};
#endif

/*For BlueTooth*/
#ifdef CONFIG_BROADCOM_RFKILL
#include <linux/bt-rfkill.h>
static void set_pin_status(int bt_power_state)
{
#if 0
	if(bt_power_state){
		/*set UART0_RXD, UART0_CTS_N ,2 pins to input nopull*/
		jzgpio_set_func(GPIO_PORT_F, GPIO_INPUT, 0x3);
		/*set UART0_TXD to output low*/
		jzgpio_set_func(GPIO_PORT_F, GPIO_INPUT, 0x8);

		/*set PCM0_DO ,PCM0_CLK, PCM0_SYN ,PCM0_DI 4 pins to OUTPUT_LOW*/
		jzgpio_set_func(GPIO_PORT_F, GPIO_OUTPUT0, 0xF << 12);
	}else{
#if defined(GPIO_BT_RST_N)
		jzgpio_set_func(GPIO_BT_RST_N / 32, GPIO_OUTPUT0,
				1 << (GPIO_BT_RST_N % 32));
#endif
		jzgpio_set_func(GPIO_BT_INT / 32, GPIO_OUTPUT0,
				1 << (GPIO_BT_INT % 32));
		jzgpio_set_func(GPIO_BT_WAKE / 32, GPIO_OUTPUT0,
				1 << (GPIO_BT_WAKE % 32));

		/*set BT_RST_N ,BT_INT, BT_WAKE , BT_REG_ON 4 pins to OUTPUT_LOW*/
		jzgpio_set_func(GPIO_BT_REG_ON / 32, GPIO_OUTPUT0,
				1 << (GPIO_BT_REG_ON % 32));

		/*set UART0_RXD, UART0_CTS_N, UART0_RTS_N 3 pins to OUTPUT_LOW*/
		jzgpio_set_func(GPIO_PORT_F, GPIO_OUTPUT0 , 0x7);

		/*set UART0_TXD to INPUT_NOPULL*/
		jzgpio_set_func(GPIO_PORT_F, GPIO_INPUT , 1 << 3);

		/*set PCM0_DO ,PCM0_CLK, PCM0_SYN ,PCM0_DI 4 pins to OUTPUT_LOW*/
		jzgpio_set_func(GPIO_PORT_F, GPIO_OUTPUT0 , 0xF << 12);
	}
#endif
}

static void restore_pin_status(int bt_power_state)
{

	/*set UART0_RXD, UART0_CTS_N, UART0_RTS_N, UART0_TXD to FUNC*/
	jzgpio_set_func(BLUETOOTH_UART_GPIO_PORT, BLUETOOTH_UART_GPIO_FUNC, BLUETOOTH_UART_FUNC_SHIFT);

	/*set PCM0_DO ,PCM0_CLK, PCM0_SYN ,PCM0_DI 4 pins to FUNC*/
	//jzgpio_set_func(GPIO_PORT_F, GPIO_FUNC_0, 0xF << 12);
}

static struct bt_rfkill_platform_data  bt_gpio_data = {
	.gpio = {
		.bt_rst_n = HOST_BT_RST,
		.bt_reg_on = BT_REG_EN,
		.bt_wake = HOST_WAKE_BT,
		.bt_int = BT_WAKE_HOST,
		.bt_uart_rts = BT_UART_RTS,
#if 0
		.bt_int_flagreg = -1,
		.bt_int_bit = -1,
#endif
	},

	.restore_pin_status = restore_pin_status,
	.set_pin_status = set_pin_status,
#if 0
	.suspend_gpio_set = NULL,
	.resume_gpio_set = NULL,
#endif
};

struct platform_device bt_power_device  = {
	.name = "bt_power" ,
	.id = -1 ,
	.dev   = {
		.platform_data = &bt_gpio_data,
	},
};

struct platform_device bluesleep_device = {
	.name = "bluesleep" ,
	.id = -1 ,
	.dev   = {
		.platform_data = &bt_gpio_data,
	},

};

#ifdef CONFIG_BT_BLUEDROID_SUPPORT
int bluesleep_tty_strcmp(const char* name)
{
	if(!strcmp(name,BLUETOOTH_UPORT_NAME)){
		return 0;
	} else {
		return -1;
	}
}
EXPORT_SYMBOL(bluesleep_tty_strcmp);
#endif
#endif /* CONFIG_BROADCOM_RFKILL */

/*For NFC*/
#ifdef CONFIG_BCM2079X_NFC
#include <linux/nfc/bcm2079x.h>
struct bcm2079x_platform_data bcm2079x_pdata = {
	.irq_gpio = NFC_REQ,
	.en_gpio = NFC_REG_PU,
	.wake_gpio= HOST_WAKE_NFC,
	.power_down_gpio_set = NULL,
	.power_on_gpio_set = NULL,
};
#endif	/*CONFIG_BCM2079X_NFC*/

/*For WiFi*/
#ifdef CONFIG_BCMDHD_1_141_66
#define RESET               0
#define NORMAL              1

#define WIFIMAC_ADDR_PATH "/data/misc/wifi/wifimac.txt"

static int get_wifi_mac_addr(unsigned char* buf)
{
        struct file *fp = NULL;
        mm_segment_t fs;

        unsigned char source_addr[18];
        loff_t pos = 0;
        unsigned char *head, *end;
        int i = 0;

        fp = filp_open(WIFIMAC_ADDR_PATH, O_RDONLY,  0444);
        if (IS_ERR(fp)) {
                printk("Can not access wifi mac file : %s\n",WIFIMAC_ADDR_PATH);
                return -EFAULT;
        }else{
                fs = get_fs();
                set_fs(KERNEL_DS);
                vfs_read(fp, source_addr, 18, &pos);
                source_addr[17] = ':';

                head = end = source_addr;
                for(i=0; i<6; i++) {
                        while (end && (*end != ':') )
                                end++;

                        if (end && (*end == ':') )
                                *end = '\0';

                        buf[i] = simple_strtoul(head, NULL, 16 );

                        if (end) {
                                end++;
                                head = end;
                        }
                        printk("wifi mac %02x \n", buf[i]);
                }
                set_fs(fs);
                filp_close(fp, NULL);
        }

        return 0;
}
static struct wifi_platform_data bcmdhd_wlan_pdata = {
                .get_mac_addr = get_wifi_mac_addr,
};


extern int jzmmc_manual_detect(int index, int on);
extern int jzmmc_clk_ctrl(int index, int on);
extern int bcm_power_on(void);
extern int bcm_power_down(void);

struct wifi_data {
	struct wake_lock                wifi_wake_lock;
	int                             wifi_reset;
};

struct resource wlan_resources[] = {
	[0] = {
		.start = WL_WAKE_HOST,
		.end = WL_WAKE_HOST,
		.name = "bcmdhd_wlan_irq",
		.flags  = IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHLEVEL | IORESOURCE_IRQ_SHAREABLE,
	},
};
struct platform_device wlan_device = {
	.name   = "bcmdhd_wlan",
	.id     = 1,
	.dev    = {
		.platform_data = &bcmdhd_wlan_pdata,
	},
	.resource       = wlan_resources,
	.num_resources  = ARRAY_SIZE(wlan_resources),
};

static struct wifi_data bcm_data;

static void wifi_le_set_io(void)
{
	/*when wifi is down, set WL_MSC1_D0 , WL_MSC1_D1, WL_MSC1_D2, WL_MSC1_D3,
	  WL_MSC1_CLK, WL_MSC1_CMD pins to INPUT_NOPULL status*/
	jzgpio_set_func(GPIO_PORT_E, GPIO_INPUT, 0x1 << 20);
	jzgpio_set_func(GPIO_PORT_E, GPIO_INPUT, 0x1 << 21);
	jzgpio_set_func(GPIO_PORT_E, GPIO_INPUT, 0x1 << 22);
	jzgpio_set_func(GPIO_PORT_E, GPIO_INPUT, 0x1 << 23);
	jzgpio_set_func(GPIO_PORT_E, GPIO_INPUT, 0x1 << 28);
	jzgpio_set_func(GPIO_PORT_E, GPIO_INPUT, 0x1 << 29);
}

static void wifi_le_restore_io(void)
{
	/*when wifi is up ,set WL_MSC1_D0 , WL_MSC1_D1, WL_MSC1_D2, WL_MSC1_D3,
		 WL_MSC1_CLK, WL_MSC1_CMD pins to GPIO_FUNC_0*/
	jzgpio_set_func_pull(GPIO_PORT_E, GPIO_FUNC_2, 0x1 << 20);
	jzgpio_set_func_pull(GPIO_PORT_E, GPIO_FUNC_2, 0x1 << 21);
	jzgpio_set_func_pull(GPIO_PORT_E, GPIO_FUNC_2, 0x1 << 22);
	jzgpio_set_func_pull(GPIO_PORT_E, GPIO_FUNC_2, 0x1 << 23);
	jzgpio_set_func_nopull(GPIO_PORT_E, GPIO_FUNC_2, 0x1 << 28);
	jzgpio_set_func_pull(GPIO_PORT_E, GPIO_FUNC_2, 0x1 << 29);
}

int bcm_wlan_init(void)
{
	int reset;
	wifi_le_set_io();

#if defined(HOST_WIFI_RST)
	reset = HOST_WIFI_RST;
	if (gpio_request(HOST_WIFI_RST, "wifi_reset")) {
		pr_err("no wifi_reset pin available\n");

		return -EINVAL;
	} else {
		gpio_direction_output(reset, 1);
	}
#else
	reset = -1;
#endif
	bcm_data.wifi_reset = reset;


	return 0;
}
EXPORT_SYMBOL(bcm_wlan_init);
#if 0
static int wifi_le_pull_io(void)
{
#if (defined(CONFIG_JZMMC_V12_MMC1) && defined(CONFIG_JZMMC_V12_MMC1_PE_4BIT))
#define GPIO_PORT_E_BASE     GPIO_IOBASE+0x400
#define GPIO_PORT_E_SIZE     0x100

    void __iomem *base;

    base = ioremap(GPIO_PORT_E_BASE, GPIO_PORT_E_SIZE);

    if(base == NULL) {
        printk("msc1_pull_init ioremap failed\n");
        return -1;
    }

    writel(((0x1 << 29) | (0xF << 20)), base + 0x78);

    iounmap(base);

#endif
    return 0;
}
#endif

int bcm_wlan_power_on(int flag)
{
	static struct wake_lock	*wifi_wake_lock = &bcm_data.wifi_wake_lock;
#ifdef WL_REG_EN
	int wl_reg_on	= WL_REG_EN;
#endif
#ifdef WL_RST_EN
	int reset = bcm_data.wifi_reset;
#endif

	if (wifi_wake_lock == NULL)
		pr_warn("%s: invalid wifi_wake_lock\n", __func__);
#ifdef WL_RST_EN
	else if (!gpio_is_valid(reset))
		pr_warn("%s: invalid reset\n", __func__);
#endif
	else
		goto start;

	return -ENODEV;
start:
	pr_debug("wlan power on:%d\n", flag);
	wifi_le_restore_io();
	//wifi_le_pull_io();
	bcm_power_on();

	msleep(1);

	switch(flag) {
		case RESET:
#ifdef WL_REG_EN
			gpio_direction_output(wl_reg_on,1);
			msleep(1);
#endif
			msleep(1);
#ifdef WL_RST_EN
			gpio_direction_output(reset, 0);
			msleep(1);
			gpio_direction_output(reset, 1);
			msleep(1);
#endif
			break;
		case NORMAL:
			msleep(1);
#ifdef WL_REG_EN
			gpio_request(wl_reg_on, "wl_reg_on");
			gpio_direction_output(wl_reg_on,1);
			msleep(1);
#endif
#ifdef WL_RST_EN
			gpio_direction_output(reset, 0);
			msleep(1);
			gpio_direction_output(reset, 1);
			msleep(1);
#endif
			jzmmc_manual_detect(1, 1);

			break;
	}


	return 0;
}
EXPORT_SYMBOL(bcm_wlan_power_on);

int bcm_wlan_power_off(int flag)
{
	static struct wake_lock	*wifi_wake_lock = &bcm_data.wifi_wake_lock;
#ifdef WL_REG_EN
	int wl_reg_on = WL_REG_EN;
#endif
#ifdef WL_RST_EN
	int reset = bcm_data.wifi_reset;
#endif

	if (wifi_wake_lock == NULL)
		pr_warn("%s: invalid wifi_wake_lock\n", __func__);
#ifdef WL_RST_EN
	else if (!gpio_is_valid(reset))
		pr_warn("%s: invalid reset\n", __func__);
#endif
	else
		goto start;
	return -ENODEV;
start:
	pr_debug("wlan power off:%d\n", flag);
	switch(flag) {
		case RESET:
#ifdef WL_REG_EN
			gpio_direction_output(wl_reg_on,0);
#endif
#ifdef WL_RST_EN
			gpio_direction_output(reset, 0);
#endif
			msleep(1);
			break;

		case NORMAL:
#ifdef WL_RST_EN
			gpio_direction_output(reset, 0);
#endif
			udelay(65);

			/*
			 *  control wlan reg on pin
			 */
#ifdef WL_REG_EN
			gpio_direction_output(wl_reg_on,0);
#endif
			msleep(1);
//			jzmmc_manual_detect(1, 0);
			break;
	}

	//	wake_unlock(wifi_wake_lock);

	bcm_power_down();
	wifi_le_set_io();

	return 0;
}
EXPORT_SYMBOL(bcm_wlan_power_off);
#endif
