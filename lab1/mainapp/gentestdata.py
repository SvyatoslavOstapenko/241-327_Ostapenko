import datetime
import random

from django.db import transaction
from faker import Faker

from .models import BoardGame


fk = Faker()


def gentestdata():
    with transaction.atomic():
        for _ in range(30):
            game = BoardGame()
            game.title = f"{fk.word().capitalize()} {fk.word().capitalize()}"
            game.publisher = fk.company()[:120]
            game.release_date = fk.date_between(
                start_date=datetime.date(2000, 1, 1),
                end_date=datetime.date(2025, 1, 1),
            )
            game.players_count = random.randint(1, 8)
            game.play_time_minutes = random.randint(15, 240)
            game.price = round(random.uniform(500, 15000), 2)
            game.is_cooperative = random.choice([True, False])
            game.description = fk.text(max_nb_chars=200)
            game.save()

    print("OK gentestdata()")