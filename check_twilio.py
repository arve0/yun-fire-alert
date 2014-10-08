#!/usr/bin/env python
# encoding: utf-8
"""
yun-fire-alert
Author: Arve Seljebu
Code: https://github.com/arve0/yun-fire-alert

Check if we got connection to Twilio API.
"""
import httplib2, os

# use bundled certificate file - full path = hacky, hack, better way?
http = httplib2.Http(ca_certs=os.path.abspath('/root/cacert.pem'))
http.follow_redirects = False

try:
    response, content = http.request('https://api.twilio.com/2010-04-01')
except Exception as error:
    print(error)
    exit(1)
else:
    if response.status == 200 and response['content-type'] == 'application/xml':
        exit(0)
    elif response.status != 200:
        print('Got response status {status}'.format(**response))
        exit(1)
    else:
        print('Response not xml. Content-type: {content-type}'.format(**response))
        exit(1)
