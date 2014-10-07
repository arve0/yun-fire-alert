#!/usr/bin/env python
# encoding: utf-8
'''
Send an SMS alert through Twilio.
'''
from twilio.rest.exceptions import TwilioRestException
from twilio.rest import TwilioRestClient
import smtplib
from email.mime.text import MIMEText


# put your own credentials here
ACCOUNT_SID = ''
AUTH_TOKEN = ''
FROM_NUMBER = ''

# email settings
FROM_EMAIL = ''
SMTP_SERVER = ''

# put the reciever numbers here, including +country
NUMBERS = [
        '+47XXXXXXXX',
        '+47XXXXXXXX'
        ]

# email adresses in case of twilio failing
EMAILS = [
        'you@domain.com',
        'you2@domain.com'
        ]

# alert message
MESSAGE = 'Fire alarm alert!'

# setup
client = TwilioRestClient(ACCOUNT_SID, AUTH_TOKEN)

# send SMSes
for number in NUMBERS:
    try:
        client.messages.create(
            from_=FROM_NUMBER,
            to=number,
            body=MESSAGE)
    except TwilioRestException as err:
        # error handling, send error message to adresses in EMAILS
        body = ('Message: ' + MESSAGE + '\n'
                'Twilio error message: ' + err.msg + '\n'
                'Method: ' + err.method + '\n'
                'Status: ' + str(err.status) + '\n'
                'Uri: ' + err.uri)
        msg = MIMEText(body)
        msg['Subject'] = 'Twilio Error'
        msg['From'] = FROM_EMAIL
        s = smtplib.SMTP(SMTP_SERVER)
        for adress in EMAILS:
            msg['To'] = adress
        s.sendmail(FROM_EMAIL, adress, msg.as_string())
        s.quit()
