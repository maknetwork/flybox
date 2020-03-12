from __future__ import absolute_import
import os
from celery import Celery
import csv
import urllib.request
from django.conf import settings

# set the default Django settings module for the 'celery' program.
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'flybox.settings')
app = Celery('flybox')

# Using a string here means the worker will not have to
# pickle the object when using Windows.
app.config_from_object('django.conf:settings')
app.autodiscover_tasks(lambda: settings.INSTALLED_APPS)


@app.task(bind=True)
def debug_task(self):
    print('Request: {0!r}'.format(self.request))

@app.task
def see_you():
	classb =1 
	contents = urllib.request.urlopen("http://192.168.0.106/5/on").read()
	return classb



@app.task
def pd6():
    print("it working")
@app.task
def pd9():
    print("it working")

@app.task
def pd7():
	contents = urllib.request.urlopen("http://192.168.0.106/5/on").read()
@app.task
def pd14():
    print("it working")
    contents = urllib.request.urlopen("http://192.168.0.106/5/off").read()
@app.task
def pd6():
    print("it working")
    contents = urllib.request.urlopen("http://192.168.0.107/5/on").read()
@app.task
def cd7():
    print("it working")
    contents = urllib.request.urlopen("http://192.168.0.107/5/on").read()
@app.task
def cd6():
    print("it working")
    contents = urllib.request.urlopen("http://192.168.0.107/5/on").read()