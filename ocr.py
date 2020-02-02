import httplib, urllib, base64, json, re
from os import system

#to run python ocr.py
# CHANGE {MS_API_KEY} BELOW WITH YOUR MICROSOFT VISION API KEY
ms_api_key = "3ce370692850429d98d3bfb773bc37c2"

# setup vision API
headers = {
    'Content-Type': 'application/octet-stream',
    'Ocp-Apim-Subscription-Key': ms_api_key,
}

params = urllib.urlencode({
    'visualFeatures': 'Description',
})

body = open('1.jpg', "rb").read()
conn = httplib.HTTPSConnection('centralindia.api.cognitive.microsoft.com')
conn.request("POST", "/vision/v1.0/ocr?%s" % params, body, headers)
response1 = conn.getresponse()
analysis1 = json.loads(response1.read())

arr=analysis1[u'regions'][0][u'lines']
print(len(arr))
c=0
f=open("ocr.txt","w+")
for i in arr:
	arr1=i[u'words']
	for j in arr1:
		arr2=j['text']
		print(arr2,c)
		c=c+1
		f.write(arr2.encode("utf8")+" ")
