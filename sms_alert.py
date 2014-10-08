#!/usr/bin/env python
# encoding: utf-8
'''
SMS alert through Twilio.

Usage: ./sms_alert.py (fire|failure|test)
'''

# put your twilio credentials here
ACCOUNT_SID = ''
AUTH_TOKEN = ''
FROM_NUMBER = ''

# email settings
FROM_EMAIL = ''
SMTP_SERVER = ''

# receivers of alarms and failures
NUMBERS = [
        '+47XXXXXXXX',
        '+47XXXXXXXX'
        ]
# receivers of alarms only
ALARM_NUMBERS = [
        '+47XXXXXXXX'
        ]
ALARM_NUMBERS.extend(NUMBERS)

# email adresses in case of sms failing
EMAIL_ADRESSES = [
        'you@domain.com',
        'you2@domain.com'
        ]

# alarm message
ALARM_MESSAGE = 'Fire alarm alert!'

# failure message
FAILURE_MESSAGE = 'Fire alarm failure.'

# No edit beyond this point should be necessary unless you want to change the
# behaviour of this script!

# setup
from twilio.rest.exceptions import TwilioRestException
from twilio.rest import TwilioRestClient
import smtplib
from email.mime.text import MIMEText
import sys

client = TwilioRestClient(ACCOUNT_SID, AUTH_TOKEN)

# send SMSe
def smsAlert(message, numbers):
    for number in numbers:
        try:
            client.messages.create(
                from_=FROM_NUMBER,
                to=number,
                body=message)
        except TwilioRestException as err:
            # error handling, send error message to adresses in EMAIL_ADRESSES
            body = ('Message: ' + message + '\n'
                    'Twilio error message: ' + err.msg + '\n'
                    'Method: ' + err.method + '\n'
                    'Status: ' + str(err.status) + '\n'
                    'Uri: ' + err.uri)
            msg = MIMEText(body)
            msg['Subject'] = 'Twilio Error'
            msg['From'] = FROM_EMAIL
            s = smtplib.SMTP(SMTP_SERVER)
            msg['To'] = ', '.join(EMAIL_ADRESSES)
            s.sendmail(FROM_EMAIL, EMAIL_ADRESSES, msg.as_string())
            s.quit()
            print('Twilio error: ' + err.msg)
            exit(1)
        else:
            print('Alert sent to ' + ', '.join(numbers))

if __name__ == '__main__':
    if len(sys.argv) == 2:
        if sys.argv[1] == 'fire':
            smsAlert(FIRE_MESSAGE, ALARM_NUMBERS)
        elif sys.argv[1] == 'failure':
            smsAlert(FAILURE_MESSAGE, NUMBERS)
        elif sys.argv[1] == 'test':
            smsAlert('This is an test from sent from yun-fire-alert.', NUMBERS)
        else:
            print('''Usage: [fire|failure|test]'''.format(sys.argv[0]))
    else:
        print('''Usage: [fire|failure|test]'''.format(sys.argv[0]))
