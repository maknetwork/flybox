from django.urls import path
from . import views
from django.conf.urls import url

from .views import connect
app_name = 'blog'
urlpatterns = [
# post views
path('', views.homepage, name='homepage'),
path('connect', connect.as_view(), name='connect'),
path('disconnect', views.disconnect, name='disconnect'),
url(r'^disconnect_flybox', views.disconnectflybox, name='disconnectflybox'),

path('edit_control', views.editcontrol, name='editcontrol'),

    url(r'^savedata/$', views.saveflybox, name='saveflybox'),
        url(r'^update_control/$', views.updatecontrol, name='update_control'),
                url(r'^startautomation/(?P<categ_name>\w+)/(?P<subcateg_name>\w+)/$', views.startautomation, name='start_automation'),

        url(r'^update_controlpd/$', views.updatecontrolpd, name='update_controlpd'),


]