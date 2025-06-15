from random import randint
import pygame

from src.entities.entity_base import Entity
from src.utils.utils import Point


class Zombie(Entity):
    def __init__(self, screen):
        super().__init__(screen, Point(0, 0), Point(0, 0), 20, 20)
        self.position.x = randint(0, 800)
        self.position.y = randint(0, 600)
        self.speed.x = randint(-1, 1)
        self.speed.y = randint(-1, 1)

    def draw(self):
        pygame.draw.rect(self.screen, (0, 255, 0), self.rect)

    def update(self):
        self.position.x += self.speed.x
        self.position.y += self.speed.y
        self.rect.x = self.position.x
        self.rect.y = self.position.y
