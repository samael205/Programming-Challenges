import pygame
import time, random
import sys
import json
import threading
from score_board import start_web_server

pygame.init()
threading._start_new_thread(start_web_server, ())

width = 800
height = 600

game_display = pygame.display.set_mode((width, height))
pygame.display.set_caption("Snake")
game_icon = pygame.image.load("snake.png")
pygame.display.set_icon(game_icon)

game_display_font = pygame.font.SysFont("comicsansms", 35)
game_timer_font = pygame.font.SysFont("comicsansms", 17)

block_size = 20
FPS = 15

clock = pygame.time.Clock()


def generate_fruit():
    random_x = round(random.randrange(50, width, block_size))
    random_y = round(random.randrange(50, height, block_size))
    return random_x, random_y


def draw(body):
    for coordinates in body[::]:
        pygame.draw.rect(game_display, (0, 205, 125), (coordinates[0], coordinates[1], block_size, block_size))


def score(score):
    score_text = game_display_font.render("Score: " + str(score), True, (225, 0, 0))
    game_display.blit(score_text, (0, 0))


def show_time(time):
    timer_text = game_timer_font.render("Time: " + "{0:.2f}s".format(time), True, (195, 95, 0))
    game_display.blit(timer_text, (width - 200, 0))


def save_score(username, score, time):
    with open("snake_score.json", 'r', encoding='utf-8') as read_scores:
        scores = json.load(read_scores)

    time_format = "{0:.2f}s".format(time)

    if username in scores.keys():
        scores[username].append((score, time_format))
    else:
        scores[username] = [[score, time_format]]

    with open("snake_score.json", 'w', encoding='utf-8') as append_scores:
        json.dump(scores, append_scores)


def get_username():
    name = ""
    font = pygame.font.Font(None, 50)
    while True:
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.unicode.isalpha():
                    name += event.unicode
                elif event.key == pygame.K_BACKSPACE:
                    name = name[:-1]
                elif event.key == pygame.K_RETURN:
                    return name
            elif event.type == pygame.QUIT:
                break
        game_display.fill((255, 255, 255))
        block = font.render(name, True, (0, 75, 200))
        message_screen = font.render("Enter you name", True, (0, 85, 185))
        rect = block.get_rect()
        rect.center = game_display.get_rect().center
        game_display.blit(message_screen, (width / 2 - 130, height / 2 - 100))
        game_display.blit(block, rect)
        pygame.display.flip()


def game_paused():
    paused = True

    font = pygame.font.Font(None, 38)
    message = font.render("Paused", True, (255, 0, 0))
    game_display.blit(message, game_display.get_rect().center)
    pygame.display.flip()

    while paused:
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_c:
                    paused = False
                if event.key == pygame.K_q:
                    pygame.quit()
                    sys.exit()
        clock.tick(5)


def wait_to_user_choice():
    game_display.fill((255, 255, 255))
    play_again = False
    font = pygame.font.Font(None, 38)
    tip = "C to play again, Q to quit"
    message = font.render(tip, True, (0, 85, 125))
    game_display.blit(message, (width/2 - 150, height/2))
    pygame.display.flip()
    while not play_again:
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_c:
                    play_again = True
                if event.key == pygame.K_q:
                    pygame.quit()
                    sys.exit()


def main_loop():
    username = get_username()
    username_display = pygame.font.SysFont("Verdana", 39)

    game_display.fill((0, 0, 0))

    game_over = False
    game_running = True

    fruit_x, fruit_y = generate_fruit()
    snake_length = 1
    snake_body = []
    lead_x = width / 2
    lead_y = height / 2
    lead_x_change = 0
    lead_y_change = 0
    my_score = 0
    timer_start = time.time()

    while game_running:
        while game_over:
            wait_to_user_choice()
            main_loop()

        while not game_over:
            for event in pygame.event.get():
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_LEFT:
                        lead_x_change = -block_size
                        lead_y_change = 0
                    elif event.key == pygame.K_RIGHT:
                        lead_x_change = block_size
                        lead_y_change = 0
                    elif event.key == pygame.K_UP:
                        lead_x_change = 0
                        lead_y_change = -block_size
                    elif event.key == pygame.K_DOWN:
                        lead_x_change = 0
                        lead_y_change = block_size
                    elif event.key == pygame.K_p:
                        game_paused()

            game_display.fill((0, 0, 0))

            username_text = username_display.render(username, True, (125, 75, 15))
            game_display.blit(username_text, (width / 2 - 100, 0))

            pygame.draw.rect(game_display, (205, 160, 0), (fruit_x, fruit_y, block_size, block_size))

            lead_x += lead_x_change
            lead_y += lead_y_change

            score(my_score)

            timer_end = time.time()
            game_timer = timer_end - timer_start
            show_time(game_timer)

            snake_body.append((lead_x, lead_y))

            if len(snake_body) > snake_length:
                del snake_body[0]

            if lead_x < 0:
                lead_x = width
            elif lead_x > width:
                lead_x = 0
            elif lead_y < 0:
                lead_y = height
            elif lead_y > height:
                lead_y = 0

            head = (snake_body[-1][0], snake_body[-1][-1])
            for each_body_part in snake_body[:-1]:
                if each_body_part == head:
                    save_score(username, my_score, game_timer)
                    game_over = True

            if lead_x > fruit_x and lead_x < fruit_x + block_size or lead_x + block_size > fruit_x and lead_x + block_size < fruit_x + block_size:
                if lead_y + block_size > fruit_y and lead_y + block_size < fruit_y + block_size:
                    snake_length += 1
                    fruit_x, fruit_y = generate_fruit()
                    my_score += 10
                elif lead_y > fruit_y and lead_y < fruit_y + block_size:
                    snake_length += 1
                    fruit_x, fruit_y = generate_fruit()
                    my_score += 10

            clock.tick(FPS)
            draw(snake_body)
            pygame.display.update()


main_loop()






