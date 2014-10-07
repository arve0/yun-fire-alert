yun-fire-alert
==============

SMS alert for Nobi 220i R2 fire alarm with Arduino Yun



# Install #
Login to YUN through SSH and install twilio:

```
opkg update
opkg install python-openssl
opkg install python-expat
opkg install distribute
easy_install twilio
```

Get and edit alert script:
```
curl -k https://raw.githubusercontent.com/arve0/yun-fire-alert/master/sms_alert.py > sms_alert.py
vim sms_alert.py
```

