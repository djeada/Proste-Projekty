from random import random

import pygame

from entities.entity_base import Entity
from entities.player import Player
from entities.zombie import Zombie


class Game:
    def __init__(self):
        self.screen = pygame.display.set_mode((800, 600))
        self.clock = pygame.time.Clock()
        self.font = pygame.font.SysFont("Arial", 30)
        self.player = Player(self.screen)
        self.zombies = []
        self.score = 0

    def draw(self):
        self.screen.fill((0, 0, 0))
        self.player.draw()
        for zombie in self.zombies:
            zombie.draw()
        text = self.font.render("Score: " + str(self.score), True, (255, 255, 255))
        self.screen.blit(text, (10, 10))
        text = self.font.render(
            "Health: " + str(self.player.health), True, (255, 255, 255)
        )
        self.screen.blit(text, (200, 10))
        pygame.display.flip()

    def update(self):
        self.player.update()
        # random spawn a zombie
        if random() < 0.05:
            self.zombies.append(Zombie(self.screen))

        for zombie in self.zombies:
            zombie.update()
        for zombie in self.zombies:
            if zombie.rect.colliderect(self.player.rect):
                self.zombies.remove(zombie)
                self.player.health -= 1
                return
            for bullet in self.player.bullets:
                if zombie.rect.colliderect(bullet.rect):
                    self.zombies.remove(zombie)
                    self.player.bullets.remove(bullet)
                    self.score += 1
                    return
            self.check_wall_collision(zombie)
        self.check_wall_collision(self.player)

        if self.player.health <= 0:
            self.display_game_over()

    def check_wall_collision(self, entity: Entity):
        if entity.rect.left < 0 or entity.rect.right > self.screen.get_width():
            entity.position.x = (
                entity.rect.left
                if entity.rect.left < 0
                else self.screen.get_width() - entity.rect.width
            )
            entity.speed.x *= -1
        if entity.rect.top < 0 or entity.rect.bottom > self.screen.get_height():
            entity.position.y = (
                entity.rect.top
                if entity.rect.top < 0
                else self.screen.get_height() - entity.rect.height
            )
            entity.speed.y *= -1

    def display_game_over(self):
        self.font = pygame.font.SysFont("Arial", 60)
        text = self.font.render("Game Over", True, (255, 255, 255))
        self.screen.blit(
            text,
            (
                self.screen.get_width() / 2 - text.get_width() / 2,
                self.screen.get_height() / 2 - text.get_height() / 2,
            ),
        )
        pygame.display.flip()
        pygame.time.wait(2000)
        pygame.quit()
        exit()
