import pygame

from entities.bullet import Bullet
from entities.entity_base import Entity
from settings.consts import Consts
from utils.utils import Point


class Player(Entity):
    def __init__(self, screen):
        super().__init__(screen, Point(400, 300), Point(5, 5), 20, 20)
        self.health = 3
        self.bullets = []
        self.direction = Point(1, 1)

    def draw(self):
        pygame.draw.rect(self.screen, (255, 255, 255), self.rect)
        for bullet in self.bullets:
            bullet.draw()

    def update(self):
        keys = pygame.key.get_pressed()
        if keys[pygame.K_w]:
            self.position.y -= self.speed.y
            self.direction.x = 0
            self.direction.y = -abs(self.speed.y / self.speed.y)
        if keys[pygame.K_s]:
            self.position.y += self.speed.y
            self.direction.y = abs(self.speed.y / self.speed.y)
            self.direction.x = 0
        if keys[pygame.K_a]:
            self.position.x -= self.speed.x
            self.direction.x = -abs(self.speed.x / self.speed.x)
            self.direction.y = 0
        if keys[pygame.K_d]:
            self.position.x += self.speed.x
            self.direction.x = abs(self.speed.x / self.speed.x)
            self.direction.y = 0
        if keys[pygame.K_SPACE]:
            start_x = self.position.x
            start_y = self.position.y
            if self.direction.x == -1 and self.direction.y == 0:
                start_x = self.position.x
                start_y = self.position.y + self.rect.height / 2 - Consts.BULLET_HEIGHT / 2
            elif self.direction.x == 1 and self.direction.y == 0:
                start_x = self.position.x + self.rect.width - Consts.BULLET_WIDTH
                start_y = self.position.y + self.rect.height / 2 - Consts.BULLET_HEIGHT / 2
            elif self.direction.x == 0 and self.direction.y == -1:
                start_x = self.position.x + self.rect.width / 2 - Consts.BULLET_WIDTH / 2
                start_y = self.position.y
            elif self.direction.x == 0 and self.direction.y == 1:
                start_x = self.position.x + self.rect.width / 2 - Consts.BULLET_WIDTH / 2
                start_y = self.position.y + self.rect.height - Consts.BULLET_HEIGHT

            self.bullets.append(Bullet(self.screen, Point(start_x, start_y), self.direction))
        self.rect.x = self.position.x
        self.rect.y = self.position.y

        for bullet in self.bullets:
            bullet.update()
            if bullet.life_counter <= 0:
                self.bullets.remove(bullet)
