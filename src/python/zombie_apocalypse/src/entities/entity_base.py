from abc import ABC, abstractmethod

import pygame

from utils.utils import Point


class Entity(ABC):
    def __init__(self, screen, position: Point, speed: Point, width, height):
        self.screen = screen
        self.position = position
        self.speed = speed
        self.rect = pygame.Rect(position.x, position.y, width, height)

    @abstractmethod
    def draw(self):
        pass

    @abstractmethod
    def update(self):
        pass
