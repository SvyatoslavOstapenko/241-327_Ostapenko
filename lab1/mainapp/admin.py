from django.contrib import admin

from .models import BoardGame


@admin.register(BoardGame)
class BoardGameAdmin(admin.ModelAdmin):
    list_display = (
        'id',
        'title',
        'publisher',
        'release_date',
        'players_count',
        'play_time_minutes',
        'price',
        'is_cooperative',
    )
    search_fields = ('title', 'publisher')
    list_filter = ('is_cooperative', 'release_date')