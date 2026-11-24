#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/delay.h>

/* DS3231 Registers */
#define DS1307_SEC   0x00
#define DS1307_MIN   0x01
#define DS1307_HOUR  0x02
#define DS1307_DAY   0x03
#define DS1307_DATE  0x04
#define DS1307_MON   0x05
#define DS1307_YEAR  0x06

static const char* days[]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
/* BCD to Decimal */
static int bcd_to_dec(u8 val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}


/* Read one register */
static int ds1307_read_reg(struct i2c_client *client, u8 reg)
{
    return i2c_smbus_read_byte_data(client, reg);
}
static int ds1307_write_reg(struct i2c_client *client,
                            u8 reg, u8 val)
{
/* Write one byte to DS1307 register */
    u8 buf[2];
    int ret;

    buf[0] = reg;   // Register address
    buf[1] = val;   // Data to write

    /* Send over I2C */
    ret = i2c_master_send(client, buf, 2);

    if (ret != 2) {
        dev_err(&client->dev,
                "DS1307 write failed (reg=0x%02x)\n",
                reg);
        return -EIO;
    }

    return 0;
}


/* Read full time */
static void ds1307_read_time(struct i2c_client *client)
{
    int sec, min, hour, day, date, mon, year;

    sec  = ds1307_read_reg(client, DS1307_SEC);
    min  = ds1307_read_reg(client, DS1307_MIN);
    hour = ds1307_read_reg(client, DS1307_HOUR);
    day  = ds1307_read_reg(client, DS1307_DAY);
    date = ds1307_read_reg(client, DS1307_DATE);
    mon  = ds1307_read_reg(client, DS1307_MON);
    year = ds1307_read_reg(client, DS1307_YEAR);

    if (sec < 0 || min < 0 || hour < 0) {
        dev_err(&client->dev, "RTC Read Failed\n");
        return;
    }

    sec  = bcd_to_dec(sec);
    min  = bcd_to_dec(min);
    hour = bcd_to_dec(hour);
    day  = bcd_to_dec(day);
    date = bcd_to_dec(date);
    mon  = bcd_to_dec(mon);
    year = bcd_to_dec(year);

    dev_info(&client->dev,
        "Time: %02d:%02d:%02d  Date: %02d/%02d/%d  Day:%s\n",
        hour, min, sec, date, mon, year, days[day-1]);
}
static u8 dec_to_bcd(int val)
{
    return ((val / 10) << 4) | (val % 10);
}

static void ds1307_set_time(struct i2c_client *client,
                            int hour, int min, int sec,
                            int date, int mon, int year,int day)
{
    ds1307_write_reg(client, 0x00, dec_to_bcd(sec));   // Seconds
    ds1307_write_reg(client, 0x01, dec_to_bcd(min));   // Minutes
    ds1307_write_reg(client, 0x02, dec_to_bcd(hour));  // Hours
    ds1307_write_reg(client, 0x04, dec_to_bcd(date));  // Date
    ds1307_write_reg(client, 0x05, dec_to_bcd(mon));   // Month
    ds1307_write_reg(client, 0x06, dec_to_bcd(year));  // Year (0–99)
    ds1307_write_reg(client,0x03,dec_to_bcd(day));
    dev_info(&client->dev, "RTC time set\n");
}
/* Probe */
static int ds1307_probe(struct i2c_client *client)
{
    dev_info(&client->dev, "DS1307 RTC detected\n");
/* ds1307_set_time(client,
                12,59, 59,   // HH:MM:SS
                23, 6, 26,3);  // DD/MM/YY*/

    /* Read time once */
    ds1307_read_time(client);
    return 0;
}


/* Remove */
static void ds1307_remove(struct i2c_client *client)
{
    dev_info(&client->dev, "DS1307 RTC removed\n");
}


/* Device Tree Match */
static const struct of_device_id ds1307_dt_ids[] = {
    { .compatible = "dallas,ds1307" },
    { }
};

MODULE_DEVICE_TABLE(of, ds1307_dt_ids);


/* I2C Driver */
static struct i2c_driver ds1307_driver = {
    .driver = {
        .name = "ds1307_driver",
        .of_match_table = ds1307_dt_ids,
    },
    .probe = ds1307_probe,
    .remove = ds1307_remove,
};

module_i2c_driver(ds1307_driver);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ram");
MODULE_DESCRIPTION("Simple DS1307 RTC I2C Driver");
