from django.db import models
from django.utils import timezone
import json

# Create your models here.
class Flyboxset(models.Model):
	title = models.CharField(max_length=250,null=False,unique=True)
	ipaddress = models.CharField(max_length=25, null=False)
	publish = models.DateTimeField(default=timezone.now)
	created = models.DateTimeField(auto_now_add=True)
	updated = models.DateTimeField(auto_now=True)
	def __str__(self):
		return self.title
	def get_first_image(self):
	    reviews = Automation.objects.get(flybox=self.id)

	    return reviews.setupcompleted
	def get_cd_enabled(self):
	    reviews = Automation.objects.get(flybox=self.id)

	    return reviews.cdenabled
	def get_pd_enabled(self):
	    reviews = Automationpd.objects.get(flybox=self.id)

	    return reviews.pdenabled

class Setup(models.Model):
  cdfow = models.IntegerField()
  cdbac = models.IntegerField()
  flybox = models.ForeignKey(
      Flyboxset, on_delete=models.CASCADE, related_name='blog_posts',primary_key=True)
  pdfow = models.IntegerField()
  pdbac = models.IntegerField()
  etd1fow = models.IntegerField()
  etd1bac = models.IntegerField()
  etd2fow = models.IntegerField()
  etd2bac = models.IntegerField()
  etd3fow = models.IntegerField()
  etd3bac = models.IntegerField()
  setupcompleted = models.BooleanField(default=False) 

class Automation(models.Model):

  flybox = models.ForeignKey(
      Flyboxset, on_delete=models.CASCADE, related_name='automataflyboxa',primary_key=True)

  cddays = models.CharField(max_length=200, null=True)
  cdtime = models.CharField(max_length=200, null=True)
  setupcompleted = models.BooleanField(default=False)
  cdenabled = models.BooleanField(default=False) 


  def set_cddays(self, x):
  	self.cddays = json.dumps(x)

  def get_cddays(self):
    return json.loads(self.cddays)
  def set_pddays(self, x):
  	self.pddays = json.dumps(x)

  def get_pddays(self):
    return json.loads(self.pddays)

class Automationpd(models.Model):

  flybox = models.ForeignKey(
      Flyboxset, on_delete=models.CASCADE, related_name='automatapdflyboxa',primary_key=True)
  pddtime= models.CharField(max_length=200, null=True)
  pddays = models.CharField(max_length=200, null=True)
  setupcompleted = models.BooleanField(default=False)
  pdenabled = models.BooleanField(default=False) 



  def set_cddays(self, x):
  	self.cddays = json.dumps(x)

  def get_cddays(self):
    return json.loads(self.cddays)
  def set_pddays(self, x):
  	self.pddays = json.dumps(x)

  def get_pddays(self):
    return json.loads(self.pddays)





