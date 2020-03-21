# Generated by Django 3.0.4 on 2020-03-09 14:22

from django.db import migrations, models
import django.db.models.deletion
import django.utils.timezone


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Flyboxset',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('title', models.CharField(max_length=250, unique=True)),
                ('ipaddress', models.CharField(max_length=25)),
                ('publish', models.DateTimeField(default=django.utils.timezone.now)),
                ('created', models.DateTimeField(auto_now_add=True)),
                ('updated', models.DateTimeField(auto_now=True)),
            ],
        ),
        migrations.CreateModel(
            name='Setup',
            fields=[
                ('cdfow', models.IntegerField()),
                ('cdbac', models.IntegerField()),
                ('flybox', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, primary_key=True, related_name='blog_posts', serialize=False, to='home.Flyboxset')),
                ('pdfow', models.IntegerField()),
                ('pdbac', models.IntegerField()),
                ('etd1fow', models.IntegerField()),
                ('etd1bac', models.IntegerField()),
                ('etd2fow', models.IntegerField()),
                ('etd2bac', models.IntegerField()),
                ('etd3fow', models.IntegerField()),
                ('etd3bac', models.IntegerField()),
                ('setupcompleted', models.BooleanField(default=False)),
            ],
        ),
    ]