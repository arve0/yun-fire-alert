#!/usr/bin/env python
# encoding: utf-8
"""
Send an SMS alert through Twilio.
"""
from twilio.rest import TwilioRestClient

# put your own credentials here
ACCOUNT_SID = ""
AUTH_TOKEN = ""
FROM_NUMBER = ""

# put the reciever numbers here, including +country
NUMBERS = [
        '+47XXXXXXXX'
        ]

# alert message
MESSAGE = 'Fire alarm alert!'

# setup
client = TwilioRestClient(ACCOUNT_SID, AUTH_TOKEN)

# send SMSes
for number in NUMBERS:
    client.messages.create(
            from_=FROM_NUMBER,
            to=number,
            body=MESSAGE
    )
