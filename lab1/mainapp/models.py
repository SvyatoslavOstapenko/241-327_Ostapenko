from django.db import models


class BoardGame(models.Model):
    title = models.CharField(max_length=200)
    publisher = models.CharField(max_length=120)
    release_date = models.DateField()
    players_count = models.PositiveIntegerField()
    play_time_minutes = models.PositiveIntegerField()
    price = models.DecimalField(max_digits=8, decimal_places=2)
    is_cooperative = models.BooleanField(default=False)
    description = models.TextField(blank=True)
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.title