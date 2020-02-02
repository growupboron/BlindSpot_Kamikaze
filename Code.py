import logging
import os

from flask import Flask
from flask_ask import Ask, request, session, question, statement
import RPi.GPIO as GPIO

app = Flask(__name__)
ask = Ask(app, "/")
logging.getLogger('flask_ask').setLevel(logging.DEBUG)

@ask.launch
def launch():
    speech_text = 'Welcome to BlindSpot. Your personal Assistant and friend. How can I help you?'
    #os.system("python Code.py")
    return question(speech_text).reprompt(speech_text).simple_card(speech_text)

@ask.intent('ObjectIntent')
def Object_Intent():
    os.system("python camera_image.py")
    f=open('scene.txt','r')
    message=f.read()
    return statement(message)
    f.close()
    
@ask.intent('OCRIntent')
def OCR_Intent():
    os.system("python camera_image_ocr.py")
    f=open('ocr.txt','r')
    message=f.read()
    return statement(message)
    f.close()

@ask.intent('NavIntent',mapping= {'destination':'destination'})
def Nav_Intent(destination):
    dest=destination
    os.system("python3 direction.py")
    f=open('dir.txt','r')
    message=f.read()
    return statement(message)
    f.close()

@ask.intent(PathNavIntent)
def Path_Intent():
    os.system("python servo.py")

@ask.intent('PayIntent')
def Pay_Intent():
    os.system("python rfid_read.py")
    f=open('pay.txt','r')
    message=f.read()
    return statement(message)
    f.close()

    
        
    
 
@ask.intent('AMAZON.HelpIntent')
def help():
    speech_text = 'You can say hello to me!'
    return question(speech_text).reprompt(speech_text).simple_card('HelloWorld', speech_text)


@ask.session_ended
def session_ended():
    return "{}", 200


if __name__ == '__main__':
    if 'ASK_VERIFY_REQUESTS' in os.environ:
        verify = str(os.environ.get('ASK_VERIFY_REQUESTS', '')).lower()
        if verify == 'false':
            app.config['ASK_VERIFY_REQUESTS'] = False
    app.run(debug=True)

os.system("python ms_visionapi.py")
