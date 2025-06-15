from math import sqrt
import pygame

from src.entities.entity_base import Entity
from src.settings.consts import Consts
from src.utils.utils import Point


class Bullet(Entity):
    def __init__(self, screen, position: Point, direction: Point):
        speed = Point(direction.x * 10, direction.y * 10)
        super().__init__(
            screen, position, speed, Consts.BULLET_WIDTH, Consts.BULLET_HEIGHT
        )
        self.life_counter = Consts.BULLET_RANGE // sqrt(
            self.speed.x ** 2 + self.speed.y ** 2
        )

    def draw(self):
        pygame.draw.rect(self.screen, (255, 0, 0), self.rect)

    def update(self):
        self.life_counter -= 1
        if self.life_counter <= 0:
            return
        self.position.x += self.speed.x
        self.position.y += self.speed.y
        self.rect.x = self.position.x
        self.rect.y = self.position.y
