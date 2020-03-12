from django.shortcuts import redirect, render, get_object_or_404
from django.views.generic import TemplateView
from .models import Flyboxset, Setup
from django.views.generic.edit import CreateView  # new
from django.http import HttpResponse, HttpResponseRedirect
from bs4 import BeautifulSoup

import requests 
import pytz
from django.http import HttpResponse, JsonResponse

def homepage():
    posts = Flyboxset.objects.all()
    print(posts)

homepage()