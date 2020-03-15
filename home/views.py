#!/usr/bin/python
# -*- coding: utf-8 -*-
from django.shortcuts import redirect, render, get_object_or_404
from django.views.generic import TemplateView
from .models import Flyboxset, Setup, Automation,Automationpd
from django.views.generic.edit import CreateView  # new
from django.http import HttpResponse, HttpResponseRedirect
from bs4 import BeautifulSoup
import requests
import pytz
from django.http import HttpResponse, JsonResponse
from django_celery_beat.models import CrontabSchedule, PeriodicTask
from crontab import CronTab


cron = CronTab(user='arif')# change the user name

def homepage(request):
    posts = Flyboxset.objects.all()
    poster = Setup.objects.all()

    return render(request, 'home/home.html', {'posts': posts,'poster':poster})


# Create your views here.

class connect(CreateView):  # new

    model = Flyboxset
    template_name = 'home/connect.html'
    fields = '__all__'

def disconnect(request):
    posts = Flyboxset.objects.all()
    return render(request, 'home/disconnect.html', {'posts': posts})

def editcontrol(request):
    if request.method == 'POST':
        request.session['django_timezone'] = request.POST['timezone']
        return redirect('/')

    else:
        posts = Flyboxset.objects.all()
        return render(request, 'home/control.html', {'posts': posts, 'timezones': pytz.common_timezones})


def saveflybox(request):

    # if this is a POST request we need to process the form data

    if request.method == 'POST':
        try:
            title = request.POST.get('name')
            ipaddress = request.POST.get('ipaddress')
            URL = "http://%s" %ipaddress
            r = requests.get(URL)

            soup = BeautifulSoup(r.content, 'html5lib')
            inputs=soup.find("input", {"id": "input-id"})
            if inputs['value']== "hello":
                user = Flyboxset.objects.create(title=title, ipaddress=ipaddress)
                user.save()
                return JsonResponse({'success':True})
            else:
                return HttpResponse('Device either offline or DoesNotExist')
        except Exception as e:
            return JsonResponse({'error':str(e)})

    else:

        return HttpResponseRedirect('')

def disconnectflybox(request):

    # if this is a POST request we need to process the form data

    if request.method == 'POST':
        try:
            pk = request.POST.get('custId')
            post = Flyboxset.objects.get(pk=pk)
            post.delete()
            return JsonResponse({'success':True})
        except:
            return JsonResponse({'error':"false"})


    else:

        return HttpResponseRedirect('')


def updatecontrol(request):
    if request.method == 'POST':

        cddays = request.POST.getlist('cddays[]')
        timecd = request.POST.get('timecd')
        flyboxkey = request.POST.get('flybox')
        obj = Automation(flybox=Flyboxset.objects.get(pk=flyboxkey), cddays=cddays,cdtime=timecd,setupcompleted=True)
        obj.save()
        print(timecd)
        my_string = ','.join(map(str, cddays))
        cdtimelist= timecd.split(':')
        print(cdtimelist[0])
        cronnamer= Flyboxset.objects.get(pk=flyboxkey)
        cronname= cronnamer.title
        cronnameid= cronnamer.pk
        print((cronnamer.title+"cd"))
        try:
            obj2 = PeriodicTask.objects.get(name=(cronname+"cd"))

        except PeriodicTask.DoesNotExist:
            commander= str( cdtimelist[1])+" "+ str(cdtimelist[0]) +' * * '+str(my_string)

            schedule, _ = CrontabSchedule.objects.get_or_create(minute=cdtimelist[1],hour=cdtimelist[0],day_of_week=my_string,day_of_month='*', month_of_year='*')
            PeriodicTask.objects.create(crontab=schedule, name=(cronname+"cd"), task=('flybox.celery.cd' + str(cronnameid) +'()'), enabled=False)
            job = cron.new(command= 'curl '+ cronnamer.ipaddress +"/5/on", comment=(cronnamer.title+"cd"))
            job.minute.every(str(int(cdtimelist[1])))
            job.hour.every(str(int(cdtimelist[0])))
            job.dow.on(*cddays)
            job.enable()
            cron.write()

            return HttpResponse('Successfull')
        else:
            schedule2, _ = CrontabSchedule.objects.get_or_create(minute=cdtimelist[1],hour=cdtimelist[0],day_of_week=my_string,day_of_month='*', month_of_year='*',)
            obj2.crontab=schedule2
            obj2.save
            print(str(int(cdtimelist[0])))

            return HttpResponse("Successfully updated")

def updatecontrolpd(request):
    if request.method == 'POST':

        pddays = request.POST.getlist('pddays[]')
        flyboxkey = request.POST.get('flyboxselect')
        print(flyboxkey)
        timepd = request.POST.get('timepd')
        obj = Automationpd(flybox=Flyboxset.objects.get(title=flyboxkey), pddays=pddays,pddtime=timepd,setupcompleted=True)
        obj.save()
        my_string = ','.join(map(str,pddays))
        pdtimelist= timepd.split(':')
        print(pdtimelist[0])
        cronnamer= Flyboxset.objects.get(title=flyboxkey)
        cronname= cronnamer.title
        cronnameid= cronnamer.pk

        try:
            obj2 = PeriodicTask.objects.get(name=(cronname+"pd"))

        except PeriodicTask.DoesNotExist:

            schedule, _ = CrontabSchedule.objects.get_or_create(minute=pdtimelist[1],hour=pdtimelist[0],day_of_week=my_string,day_of_month='*', month_of_year='*')
            PeriodicTask.objects.create(crontab=schedule, name=(cronname+"pd"), task=('flybox.celery.pd' + str(cronnameid) +'()'), enabled=False)
            job = cron.new(command='curl '+ cronnamer.ipaddress +"/5/on", comment=(cronnamer.title+"pd"))
            job.minute.every(str(int(pdtimelist[1])))
            job.hour.every(str(int(pdtimelist[0])))
            job.dow.on(*pddays)

            job.enable(False)
            cron.write()


            return HttpResponse('Successfull')

        else:
            schedule2, _ = CrontabSchedule.objects.get_or_create(minute=pdtimelist[1],hour=pdtimelist[0],day_of_week=my_string,day_of_month='*', month_of_year='*',)
            obj2.crontab=schedule2
            obj2.save
            return HttpResponse("Successfully updated")



    else:

        return HttpResponseRedirect('')

def startautomation(request):

    id = request.GET.get('dept_id')
    buttonpressed = request.GET.get('user_name')

    cronnamer= Flyboxset.objects.get(pk=id)

    pk= cronnamer.title
    if buttonpressed=="startcd" :
        Automation.objects.filter(flybox=Flyboxset.objects.get(pk=id)).update(cdenabled=True)

        obj2 = PeriodicTask.objects.get(name=(pk+"cd"))
        findjob= cron.find_comment(cronnamer.title+"cd")
        for item1 in findjob:
            print(item1)
            item1.enable(True)

        print("startcd is s")

    elif buttonpressed=="stopcd":
        Automation.objects.filter(flybox=Flyboxset.objects.get(pk=id)).update(cdenabled=False)

        obj2 = PeriodicTask.objects.get(name=(pk+"cd"))
        findjob= cron.find_comment(cronnamer.title+"cd")
        for item1 in findjob:
            print(item1)
            item1.enable(False)
        print("stopcd")

    elif buttonpressed=="startpd":
        Automationpd.objects.filter(flybox=Flyboxset.objects.get(pk=id)).update(pdenabled=True)

        obj2 = PeriodicTask.objects.get(name=(pk+"pd"))
        findjob= cron.find_comment(cronnamer.title+"pd")
        for item1 in findjob:
            print(item1)
            item1.enable(True)

        print("startcd is s")

    elif buttonpressed=="stoppd":
        Automationpd.objects.filter(flybox=Flyboxset.objects.get(pk=id)).update(pdenabled=False)

        obj2 = PeriodicTask.objects.get(name=(pk+"pd"))
        findjob= cron.find_comment(cronnamer.title+"pd")
        for item1 in findjob:
            print(item1)
            item1.enable(False)



    print(buttonpressed)
    return HttpResponseRedirect('/')
