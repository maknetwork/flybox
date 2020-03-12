from celery.task.schedules import crontab
from celery.decorators import periodic_task
from celery.utils.log import get_task_logger
from datetime import datetime, date
from django.utils import timezone
import csv
from celery import task

logger = get_task_logger(__name__)


@task
def task_save_latest_flickr_image():
    """
    Saves latest image from Flickr
    """
    print("task perform")
    logger.info("Saved image from Flickr")
    with open('innovators.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["SN", "Name", "Contribution"])
    writer.writerow([1, "Linus Torvalds", "Linux Kernel"])
    writer.writerow([2, "Tim Berners-Lee", "World Wide Web"])
    writer.writerow([3, "Guido van Rossum", "Python Programming"])