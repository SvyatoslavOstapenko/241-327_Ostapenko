from django.contrib import admin
from django.urls import path

from mainapp.views import BoardGameViewSet


urlpatterns = [
    path('admin/', admin.site.urls),

    path('boardgame', BoardGameViewSet.as_view({
        'get': 'list',
        'post': 'create',
    })),

    path('boardgame/<int:pk>', BoardGameViewSet.as_view({
        'get': 'retrieve',
        'put': 'update',
        'patch': 'partial_update',
        'delete': 'destroy',
    })),
]