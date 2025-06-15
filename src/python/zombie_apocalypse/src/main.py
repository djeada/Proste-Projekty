import pygame

from src.game.game import Game


def main():
    pygame.init()
    game = Game()
    clock = pygame.time.Clock()
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()
        game.draw()
        game.update()
        clock.tick(60)


if __name__ == "__main__":
    main()
