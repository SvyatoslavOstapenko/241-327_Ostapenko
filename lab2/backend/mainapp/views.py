from rest_framework import viewsets

from .models import BoardGame
from .serializers import BoardGameSerializer


class BoardGameViewSet(viewsets.ModelViewSet):
    queryset = BoardGame.objects.all()
    serializer_class = BoardGameSerializer