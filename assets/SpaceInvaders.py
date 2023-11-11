from tkinter import*
from tkinter import messagebox
import random
import time
import pygame

class SpaceInvader(Tk):
    """La classe SpaceInvader est donc la fenêtre tkinter et va nous permettre de créer et de gérer toute la partie."""

    def __init__(self):
        super().__init__()

        self.title("SpaceInvader")
        self.minsize(500, 500)
        #self.iconbitmap('Space_Invaders_Logo.ico')
        self.attributes('-fullscreen', True)
        self.attributes('-topmost',True)
        self.bind('<Escape>', self.exit)

        pygame.mixer.init()
        pygame.mixer.music.load('SFX/music.wav')
        pygame.mixer.music.set_volume(0.06)

        self.channel_ennemi = pygame.mixer.Channel(0)
        self.channel_ennemi_explode = pygame.mixer.Channel(1)
        self.channel_player = pygame.mixer.Channel(2)
        self.channel_player_damage = pygame.mixer.Channel(3)
        self.channel_menu = pygame.mixer.Channel(4)

        self.sound_click = pygame.mixer.Sound('SFX/click.wav')
        self.sound_click.set_volume(0.2)
        self.sound_alarm = pygame.mixer.Sound('SFX/battle_alarm.wav')
        self.sound_alarm.set_volume(0.2)
        self.sound_laser_blue = pygame.mixer.Sound('SFX/laser_blue.wav')
        self.sound_laser_blue.set_volume(0.1)
        self.sound_seismic_charge = pygame.mixer.Sound('SFX/seismic_charge.wav')
        self.sound_seismic_charge.set_volume(0.1)
        self.sound_player_explode = pygame.mixer.Sound('SFX/player_explode.wav')
        self.sound_player_explode.set_volume(0.6)
        self.sound_ennemi_explode = pygame.mixer.Sound('SFX/ennemi_explode.wav')
        self.sound_ennemi_explode.set_volume(0.06)
        self.sound_laser = pygame.mixer.Sound('SFX/laser.wav')
        self.sound_laser.set_volume(0.04)
        self.sound_devastator = pygame.mixer.Sound('SFX/devastator.wav')
        self.sound_devastator.set_volume(0.1)
        self.sound_plazma = pygame.mixer.Sound('SFX/plazma.wav')
        self.sound_plazma.set_volume(0.6)
        self.sound_bomber = pygame.mixer.Sound('SFX/bomber.wav')
        self.sound_bomber.set_volume(0.2)

        self.gameover = [PhotoImage(file='game_over.gif', format = 'gif -index %i' %(i)) for i in range(5)]
        self.reloading = [PhotoImage(file='Reloading.gif', format = 'gif -index %i' %(i)) for i in range(9)]
        self.image_wave = PhotoImage(file='Space Age/wave.png')
        self.power_up_list = [['heal', [i for i in range(1, 11)]],
                                ['full_heal', [i for i in range(1, 21)]],
                                ['shield', [i for i in range(1, 11)]],
                                ['full_shield', [i for i in range(1, 21)]],
                                ['plazma', [i for i in range(1, 26)]],
                                ['devastator', [i for i in range(1, 26)]],
                                ['laser', [i for i in range(1, 26)]],
                                ['bomber', [i for i in range(1, 26)]],
                                ['invincibility', [i for i in range(1, 36)]],
                                ['mega_heal', [i for i in range(1, 31)]],
                                ['unlimited_power', [i for i in range(1, 51)]]]

        self.explosion_list = [PhotoImage(file='Meteor/explosion_{}.png'.format(i)) for i in range(9)]
        self.explosion_bomber_list = [PhotoImage(file='Explosion/explosion_{}-bomber.png'.format(i)) for i in range(9)]

        self.image_background = PhotoImage(file='background.gif')
        self.image_background_2 = PhotoImage(file='background1.gif')
        self.explosion_image = PhotoImage(file='Explosion/explosion.png')
        self.image_sfx_on = PhotoImage(file='sfx_on.png')
        self.image_sfx_off = PhotoImage(file='sfx_off.png')
        self.image_music_on = PhotoImage(file='music_on.png')
        self.image_music_off = PhotoImage(file='music_off.png')

        self.wave = 1
        self.liste_wave = [self.wave_1, self.wave_2]
        self.liste_wave_special = [self.wave_3]
        self.score = 0
        self.old_score = 0
        self.meteor = {}
        self.compteur_meteor = 0
        self.can_shoot = True
        self.is_sfx = True
        self.is_music = True

        self.ingame = True
        self.developer = False
        self._image_explosion = None
        self._image_explosion_bomber = None
        self.over = None

        self.move_shot = {}
        self.bot_shot = {}
        self.power_up = {}
        self.compteur = 0
        self.compteur_shoot_bot = 0
        self.compteur_power = 0

        self.start()

    def start(self):
        """La méthode start creer le menu du jeu offrant le choix à l'utilisateur de quitter ou de lancer la partie."""

        self.canva = Canvas(self, bg='black', width=self.winfo_screenwidth(), height=self.winfo_screenheight())
        self.canva.pack()

        pygame.mixer.music.play(-1)

        self.move_back = self.canva.create_image(self.winfo_screenwidth()//2, self.winfo_screenheight()//2, image=self.image_background_2)

        self.move_back_2 = self.canva.create_image(self.winfo_screenwidth()//2, self.winfo_screenheight()//2-1080, image=self.image_background)

        self.background_moving()
        self.background_moving_2()

        self.image_title = PhotoImage(file='Space Age/title.png')
        self.title = self.canva.create_image(self.winfo_screenwidth()//2, self.winfo_screenheight()//4, image=self.image_title)

        self.image_founders = PhotoImage(file='Space Age/founders.png')
        self.founders = self.canva.create_image(self.winfo_screenwidth()-285, self.winfo_screenheight()-60, image=self.image_founders)

        self.image_info = PhotoImage(file='Space Age/info.png')
        self.info = self.canva.create_image(self.winfo_screenwidth()//2, self.winfo_screenheight()//2+100, image=self.image_info)

        self.image_version = PhotoImage(file='Space Age/version.png')
        self.version = self.canva.create_image(self.winfo_screenwidth()-100, 30, image=self.image_version)

        self.sfx = self.canva.create_image(100, self.winfo_screenheight()-80, image=self.image_sfx_on, tags='on')
        self.music = self.canva.create_image(300, self.winfo_screenheight()-80, image=self.image_music_on, tags='on')

        self.bind('<Return>', self.initialisation)
        self.canva.tag_bind(self.sfx, '<Button-1>', self.sfx_bind)
        self.canva.tag_bind(self.music, '<Button-1>', self.music_bind)

        self.bind('<Alt-h>', self.developer_mode)
        self.bind('<Alt-c>', self.player_mode)

    def initialisation(self, event):
        """La méthode initialisation va permetre de mettre en place tout ce qui est nécessaire au lancement de la partie."""

        self.stop_music()
        if self.is_sfx:
            self.channel_menu.play(self.sound_click)


        self.config(cursor="none")

        self.canva.delete(self.title,
                            self.founders,
                            self.info,
                            self.version,
                            self.sfx,
                            self.music)


        self.create_space_ship()

        self.reload = []

        self.reloading_gif(0, True)

        self.print_score = self.canva.create_text(15, 15, text=self.score, fill='white', font=('Times New Roman', 30), justify='left', anchor='nw')
        self.print_wave = self.canva.create_text(100, self.winfo_screenheight()-30, text='Wave: {}'.format(self.wave), fill='white', font=('Times New Roman', 30), justify='left')

        self.timer = Timer(self)
        self.timer.Start()

        self.canva.bind('<Button-1>', self.shoot)
        self.canva.bind('<Motion>', self.space_ship.player_moving)

        if self.developer:
            self.bind('<Alt-b>', self.developer_bomber)
            self.bind('<Alt-d>', self.developer_devastator)
            self.bind('<Alt-l>', self.developer_laser)
            self.bind('<Alt-p>', self.developer_plazma)
            self.bind('<Alt-s>', self.developer_shied)
            self.bind('<Alt-h>', self.developer_health)
            self.bind('<Alt-i>', self.developer_invincibility)
            self.bind('<Alt-u>', self.developer_unlimited_power)
            self.bind('<Alt-m>', self.developer_upgrade_max)

        self.begin()

    def stop_music(self):
        pygame.mixer.music.stop()

    def play_music(self):
        if self.is_music:
            pygame.mixer.music.play(-1)

    def sfx_bind(self, event):
        if self.canva.gettags(self.sfx)[0] == 'on':
            self.is_sfx = False
            self.canva.itemconfigure(self.sfx, tags='off', image=self.image_sfx_off)

        elif self.canva.gettags(self.sfx)[0] == 'off':
            self.is_sfx = True
            self.canva.itemconfigure(self.sfx, tags='on', image=self.image_sfx_on)

    def music_bind(self, event):
        if self.canva.gettags(self.music)[0] == 'on':
            self.is_music = False
            self.stop_music()
            self.canva.itemconfigure(self.music, tags='off', image=self.image_music_off)

        elif self.canva.gettags(self.music)[0] == 'off':
            self.is_music = True
            self.play_music()
            self.canva.itemconfigure(self.music, tags='on', image=self.image_music_on)

    def background_moving(self):
        """La méthode background moving permet de faire défiler la première image de fond."""

        coord = self.canva.coords(self.move_back)

        if coord[-1] == self.winfo_screenheight()//2 + 1080.0:
            self.canva.move(self.move_back, 0, -2160)
        else:
            self.canva.move(self.move_back, 0, 1)
        self.after(20, self.background_moving)

    def background_moving_2(self):
        """La méthode background moving 2 permet de faire défiler la deuxième image de fond."""

        coord = self.canva.coords(self.move_back_2)

        if coord[-1] == self.winfo_screenheight()//2 + 1080.0:
            self.canva.move(self.move_back_2, 0, -2160)
        else:
            self.canva.move(self.move_back_2, 0, 1)
        self.after(20, self.background_moving_2)

    def create_space_ship(self):
        """La méthode create space ship permet de créer le vaisseau piloté par le joueur"""

        self.space_ship = SpaceShip(self, 'laser_blue')

    def shoot(self, event):
        """La méthode évenementiellle shoot permet de faire tirer le vaisseau à chaques fois
            que l'utilisateur clique sur le clique gauche de la souris."""

        if self.space_ship is not None and len(self.move_shot) < self.space_ship._limte and self.can_shoot:
            if self.space_ship.type_space_ship == 'player_laser' and self.is_sfx:
                self.channel_player.play(self.sound_laser_blue)

            elif self.space_ship.type_space_ship == 'player_devastator' and self.is_sfx:
                self.channel_player.play(self.sound_devastator)

            elif self.space_ship.type_space_ship == 'player_plazma' and self.is_sfx:
                self.channel_player.play(self.sound_plazma)

            elif self.space_ship.type_space_ship == 'player_bomber' and self.is_sfx:
                self.channel_player.play(self.sound_bomber)

            self.can_shoot = False
            self.after(self.space_ship._cooldown, self.cooldown)

            coord = self.canva.coords(self.space_ship._move_space_ship)

            shot = Shot(self, coord, self.compteur, ['player', self.space_ship._tag])
            self.move_shot[self.compteur] = shot
            shot.shot_moving()

            self.compteur += 1

            self.reloading_gif(0)

    def change_wave(self):
        """La méthode change wave permet d'afficher à l'utilisateur la future vague qui va être lancé."""

        number = str(self.wave)
        gap = 150

        self.next_wave = self.canva.create_image(self.winfo_screenwidth()//2-120, self.winfo_screenheight()//2, image=self.image_wave)
        self.total = []

        self.number = [PhotoImage(file='Space Age/%i.png' %(int(i))) for i in number]
        for iid in self.number:
            self.total.append(self.canva.create_image(self.winfo_screenwidth()//2+gap, self.winfo_screenheight()//2, image=iid))
            gap += 100

        self.score += self.wave*1000
        self.update_score()

        self.after(5000, self.chose_wave)

    def begin(self):
        """La méthode begin permet de lancer la toute première vague de la partie."""
        if self.is_sfx:
            self.sound_alarm.play()

        self.next_wave = self.canva.create_image(self.winfo_screenwidth()//2-120, self.winfo_screenheight()//2, image=self.image_wave)
        self.number = PhotoImage(file='Space Age/1.png')
        self.total = self.canva.create_image(self.winfo_screenwidth()//2+150, self.winfo_screenheight()//2, image=self.number)

        self.after(5000, self.begin_2)

    def begin_2(self):
        """La méthode begin 2 permet d'enlever l'affichage de la vague au début de la partie."""

        pygame.mixer.music.load('SFX/sound.wav')
        pygame.mixer.music.set_volume(0.08)
        self.play_music()

        self.canva.delete(self.next_wave, self.total)

        self.wave_1()

    def chose_wave(self):
        """La méthode chose wave permet de choisir aléatoirement la prochaine vague qui va être lancé."""

        self.canva.delete(self.next_wave)

        for iid in self.total:
            self.canva.delete(iid)

        if self.wave %5 ==0:
            wave_random = random.choice(self.liste_wave_special)
        else:
            wave_random = random.choice(self.liste_wave)
        wave_random()

    def chose_power_up(self, coord):
        """La méthode chose power up permet de choisir aléatoirement si un power up et lequel, va être drop à la mort d'un ennemi."""

        power_up = random.choice(self.power_up_list)

        number = random.choice(power_up[-1])

        if number == 1:
            self.power_up[self.compteur_power] = PowerUp(self, power_up[0], coord, self.compteur_power)
            self.compteur_power += 1

    def wave_1(self):
        """La méthode wave 1 permet de creer une vague avec une certaine configuration type des ennemies."""

        coord = [-100, 100]
        self.bot = {}

        for loop in range(1, 19):
            self.bot[loop] = Bot(self, coord, 5, loop)
            if loop%6 == 0:
                coord[-1] += 100
                coord[0] = -100
            else:
                coord[0] -= 100

        coord = [2020, 150]

        for loop in range(19, 37):
            self.bot[loop] = Bot(self, coord, -5, loop)

            if loop%6 == 0:
                coord[-1] += 100
                coord[0] = 2020
            else:
                coord[0] += 100

        for bot in self.bot.values():
            bot.bot_moving_1()

    def wave_2(self):
        """La méthode wave 2 permet de creer une vague avec une certaine configuration type des ennemies."""

        coord = [660, -500]
        self.bot = {}

        for loop in range(1, 16):
            self.bot[loop] = Bot(self, coord, 5, loop)

            self.bot[loop].bot_moving_2(coord[-1]+550, True)

            if loop % 3 == 0:
                coord[0] = 660
                coord[-1] += 100
            else:
                coord[0] += 100

        coord = [960, -500]

        for loop in range(16, 31):
            self.bot[loop] = Bot(self, coord, 5, loop)

            self.bot[loop].bot_moving_2(coord[-1]+550, False)

            if loop % 3 == 0:
                coord[0] = 960
                coord[-1] += 100
            else:
                coord[0] += 100

    def wave_3(self):
        """La méthode wave 3 permet de lancer une vague spécial où l'utilisateur doit esquiver des météroites."""

        if self.compteur_meteor < 2000:
            self.meteor[self.compteur_meteor] = Meteor(self, self.compteur_meteor)
            self.compteur_meteor += 1

            self.after(25, self.wave_3)
        else:
            self.wave += 1
            self.update_wave()
            self.compteur_meteor = 0
            self.after(1000, self.change_wave)

    def game_over(self, i):
        """La méthode game over permet d'afficher le gif game over quand le joueur à perdue."""

        self.canva.delete(self.over)
        self.over = self.canva.create_image(self.winfo_screenwidth()//2, self.winfo_screenheight()//2, image=self.gameover[i%5])

        self.after(100, self.game_over, i+1)

    def reloading_gif(self, i, begining=False):
        """La méthode reloading gif permet d'afficher le gif reloading qui affiche à l'utilisateur quand il peut tirer avec son arme."""
        if self.space_ship != None:
            if i <= 8:
                if i == 0:
                    for iid in self.reload:
                        self.canva.delete(iid)
                self.reload.append(self.canva.create_image(self.winfo_screenwidth()-50, self.winfo_screenheight()-70, image=self.reloading[i]))
                if begining:
                    self.after(0, self.reloading_gif, i+1, True)
                else:
                    self.after(self.space_ship._cooldown//8, self.reloading_gif, i+1)

    def update_score(self, coord=[30, 60]):
        """La méthode update score permet de mettre à jour le score affiché dans l'HUD."""

        self.after(200, self.new_update_score, coord, self.score-self.old_score)
        self.old_score = self.score

        coord_temp = self.canva.coords(self.print_score)

        self.canva.itemconfigure(self.print_score, text=self.score)
##        if len(str(self.score//100)) >= coord_temp[0]//15:
##            self.canva.move(self.print_score, 15, 0)

    def new_update_score(self, coord, score):
        """La méthode permet d'afficher dans le HUD si il y a eut un bonus de score et combien."""

        self.print_new_score = self.canva.create_text(coord[0], coord[-1], text='+ {}'.format(score), fill='white', font=('Times New Roman', 15), justify='left')
        self.after(500, self.canva.delete, self.print_new_score)

    def update_wave(self):
        """La méthode update wave permet de mettre à jour la vague afficher dans l'HUD."""

        self.canva.itemconfigure(self.print_wave, text='Wave: {}'.format(self.wave))

    def exit(self, event):
        """La méthode évenementielle exit permet de fermet la fenêtre et le programme quand l'utilisateur appuye sur la touche ECHAP."""
        if self.is_sfx:
            self.channel_menu.play(self.sound_click)

        pygame.mixer.music.stop()
        self.destroy()

    def cooldown(self):
        """La méthode cooldow permet à l'utilisateur de tirer à nouveau."""

        self.can_shoot = True

    def explosion(self, coord, i=0):
        """La méthode explosion permet d'afficher les images de l'explosions causé par la destruction d'une météroite."""

        if i <= 8:
            self.canva.delete(self._image_explosion)

            self._image_explosion = self.canva.create_image(coord[0], coord[-1], image=self.explosion_list[i])

            self.after(100, self.explosion, coord,i+1)

    def explosion_bomber(self, coord, i=0):
        """La méthode explosion bomber permet d'afficher les images de l'explosion causé par le tir du vaisseau bomber."""

        if i <= 8:
            self.canva.delete(self._image_explosion_bomber)

            self._image_explosion_bomber = self.canva.create_image(coord[0], coord[-1], image=self.explosion_bomber_list[i])

            self.after(100, self.explosion_bomber, coord,i+1)

    def developer_mode(self, event):
        """La méthode évenementielle developer mode permet d'activer le mode dévelopeur quand l'utilisateur appuye sur ALT + H dans le menu."""

        self.developer = True

        messagebox.showinfo('Developer Mode', 'You have just activated the developer mode.\n To deactivate it, please press ALT + C')

    def player_mode(self, event):
        """La méthode évenementielle player mode permet de désactiver le mode dévelopeur quand l'utilisateur appuye sur ALT + C dans le menu."""
        self.developer = False

        messagebox.showinfo('Player Mode', 'You have just desactivated the developer mode.\n To activate it, please press ALT + H')

    def developer_bomber(self, event):
        """La méthode évenementielle developer bomber permet quand le mode dévelopeur est activé
            de mettre le type de vaisseau en mode bomber quand l'utilisateur appuye sur ALT + B."""

        self.space_ship.bomber()

    def developer_laser(self, event):
        """La méthode évenementielle developer laser permet quand le mode dévelopeur est activé
            de mettre le type de vaisseau en mode laser quand l'utilisateur appuye sur ALT + L."""

        self.space_ship.laser()

    def developer_devastator(self, event):
        """La méthode évenementielle developer devastator permet quand le mode dévelopeur est activé
            de mettre le type de vaisseau en mode devastator quand l'utilisateur appuye sur ALT + D."""

        self.space_ship.devastator()

    def developer_plazma(self, event):
        """La méthode évenementielle developer plazma permet quand le mode dévelopeur est activé
            de mettre le type de vaisseau en mode plazma quand l'utilisateur appuye sur ALT + p."""

        self.space_ship.plazma()

    def developer_shied(self, event):
        """La méthode évenementielle developer shield permet quand le mode dévelopeur est activé
            de remplir la protection du vaisseeau quand l'utilisateur appuye sur ALT + S."""

        self.space_ship._shield = 3
        self.space_ship.health()

    def developer_health(self, event):
        """La méthode évenementielle developer health permet quand le mode dévelopeur est activé
            de remplir la santé du vaisseau quand l'utilisateur appuye sur ALT + H."""

        self.space_ship._health = 3
        self.space_ship.health()

    def developer_invincibility(self, event):
        """La méthode évenementielle developer invincibility permet quand le mode dévelopeur est activé
            de rendre le vaisseau invincible pendant 15 secondes quand l'utilisateur appuye sur ALT + I."""

        self.space_ship.active_invincibility()

    def developer_upgrade_max(self, event):
        """La méthode évenementielle developer upgrade max permet quand le mode dévelopeur est activé
            de mettre le niveau de l'arme du vaisseau au niveau max quand l'utilisateur appuye sur ALT + M."""

        self.space_ship.upgrad_max()

    def developer_unlimited_power(self, event):
        """La méthode évenementielle developer unlimited power permet quand le mode dévelopeur est activé
            de donner le bonus ulimited power au vaisseau quand l'utilisateur appuye sur ALT + U."""

        self.space_ship._health = 3
        self.space_ship._shield = 3
        self.space_ship.health()

        self.space_ship.upgrad_max()

        self.space_ship.active_invincibility()

    def restart(self, event):
        """La méthode évenementielle restart permet de relancer la partie quand l'utilisateur appuye sur ENTREE après avoir perdu."""

        self.channel_menu.play(self.sound_click)

        self.destroy()

        game= SpaceInvader()
        game.mainloop()

class SpaceShip():
    """La classe SpaceShip permet de creer le vaisseau qui va être piloté par le joueur.
        Elle va aussi gérer sa santé et tout les diffentes actions ou bonus liés au vaisseau."""

    def __init__(self, window, tag, healthShip=3, shieldShip=0):

        self.type_space_ship = 'player_laser'
        self._image = PhotoImage(file='SpaceShip/{}.gif'.format(self.type_space_ship))
        self._health = healthShip
        self._shield = shieldShip
        self._weapon_level = 1
        self.invincible = False

        self._tag = tag
        self._limte = 5
        self._cooldown = 700

        self._window = window


        self._move_space_ship = self._window.canva.create_image(self._window.winfo_screenwidth()//2, 810 , image=self._image, tags='player')
        self.print_level = self._window.canva.create_text(self._window.winfo_screenwidth()-140, self._window.winfo_screenheight()-30, text='Weapon level: {}'.format(self._weapon_level), fill='white', font=('Times New Roman', 30), justify='left')

        file = 'HP/Life-{}-{}.png'.format(self._health, self._shield)
        self.image_health_bar = PhotoImage(file=file)
        self.health_bar = self._window.canva.create_image(self._window.winfo_screenwidth()//2, self._window.winfo_screenheight()-15, image=self.image_health_bar)

    def player_moving(self, event):
        """La méthode évenementielle player moving permet au vaisseau de suivre la souris de l'utilisateur."""

        if self._window.ingame:
            coord = self._window.canva.coords(self._move_space_ship)

            x = event.x - coord[0]
            y = event.y - coord[-1]

            self._window.canva.move(self._move_space_ship, x, y)

    def health(self):
        """la méthode health permet de gérer l'affichage de la santé de l'utilisateur dans le HUD.
            Elle va aussi mettre fin à la partie et lancer le game over quand la santé du joueur atteint 0."""

        if self._health == 0:
            if self._window.is_sfx:
                self._window.channel_player_damage.play(self._window.sound_player_explode)

            self._window.timer.Stop()
            self._window.canva.delete(self._move_space_ship)
            self._window.canva.delete(self.health_bar)
            self._window.space_ship = None
            self._window.ingame = False
            self._window.game_over(0)

            self.image_restart = PhotoImage(file='Space Age/restart.png')
            self.restart = self._window.canva.create_image(self._window.winfo_screenwidth()//2, (self._window.winfo_screenheight()//4)*3, image=self.image_restart)

            self._window.bind('<Return>', self._window.restart)

        else:
            file = 'HP/Life-{}-{}.png'.format(self._health, self._shield)
            self.image_health_bar = PhotoImage(file=file)
            self._window.canva.itemconfigure(self.health_bar, image=self.image_health_bar)

            if self._shield > 0:
                self._image = PhotoImage(file='SpaceShip/{}_shield.gif'.format(self.type_space_ship))
                self._window.canva.itemconfigure(self._move_space_ship, image=self._image)

            elif self._shield == 0:
                self._image = PhotoImage(file='SpaceShip/{}.gif'.format(self.type_space_ship))
                self._window.canva.itemconfigure(self._move_space_ship, image=self._image)

            if self.invincible:
                self._image = PhotoImage(file='SpaceShip/{}_invincible.gif'.format(self.type_space_ship))
                self._window.canva.itemconfigure(self._move_space_ship, image=self._image)

    def plazma(self):
        """La méthode plazma permet de changer le type de tir vaisseau par plazma."""

        if self.type_space_ship != 'player_plazma':
            self._tag = 'plazma'

            self.type_space_ship = 'player_plazma'
            self._image = PhotoImage(file='SpaceShip/{}.gif'.format(self.type_space_ship))
            self._window.canva.itemconfigure(self._move_space_ship, image=self._image)

            self.health()
        else:
            if self._weapon_level < 3:
                self._weapon_level += 1
            else:
                self._window.score += 3000
                self._window.update_score()
        self.upgrade_plazma()

    def upgrade_plazma(self):
        """La méthode upgrade plazma permet d'améliorer le type de tir plazma du vaisseau."""

        if self._weapon_level ==1:
            self._cooldown = 700
            self._limte = 3
            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self._weapon_level == 2:
            self._cooldown = 600
            self._limte = 4
            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self._weapon_level ==3:
            self._cooldown = 500
            self._limte = 5
            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))


    def devastator(self):
        """La méthode devastator permet de changer le type de tir vaisseau par devastator."""

        if self.type_space_ship != 'player_devastator':
            self.type_space_ship = 'player_devastator'
            self._tag = 'devastator'

            self._image = PhotoImage(file='SpaceShip/{}.gif'.format(self.type_space_ship))
            self._window.canva.itemconfigure(self._move_space_ship, image=self._image)

            self.health()
        else:
            if self._weapon_level < 3:
                self._weapon_level += 1
            else:
                self._window.score += 3000
                self._window.update_score()

        self.upgrade_devastator()

    def upgrade_devastator(self):
        """La méthode upgrade devastator permet d'améliorer le type de tir devastator du vaisseau."""

        if self._weapon_level ==1:
            self._limte = 1
            self._cooldown = 4000

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self._weapon_level == 2:
            self._cooldown = 3500

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self._weapon_level == 3:
            self._cooldown = 3000

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

    def laser(self):
        """La méthode laser permet de changer le type de tir vaisseau par laser."""

        if self.type_space_ship != 'player_laser':
            self.type_space_ship = 'player_laser'
            self._tag = 'laser_blue'

            self._image = PhotoImage(file='SpaceShip/{}.gif'.format(self.type_space_ship))
            self._window.canva.itemconfigure(self._move_space_ship, image=self._image)


            self.health()
        else:
            if self._weapon_level < 3:
                self._weapon_level += 1
            else:
                self._window.score += 3000
                self._window.update_score()

        self.upgrade_laser()

    def upgrade_laser(self):
        """La méthode upgrade laser permet d'améliorer le type de tir laser du vaisseau."""

        if self._weapon_level == 1:
            self._limte = 5
            self._cooldown = 700

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self._weapon_level == 2:
            self._limte = 6
            self._cooldown = 600

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self._weapon_level == 3:
            self._limte = 7
            self._cooldown = 500

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

    def bomber(self):
        """La méthode bomber permet de changer le type de tir vaisseau par bomber."""

        if self.type_space_ship != 'player_bomber':
            self._tag = 'bomber'
            self.type_space_ship = 'player_bomber'

            self._image = PhotoImage(file='SpaceShip/{}.gif'.format(self.type_space_ship))
            self._window.canva.itemconfigure(self._move_space_ship, image=self._image)

            self.health()
        else:
            if self._weapon_level < 3:
                self._weapon_level += 1
            else:
                self._window.score += 3000
                self._window.update_score()

        self.upgrade_bomber()

    def upgrade_bomber(self):
        """La méthode upgrade bomber permet d'améliorer le type de tir bomber du vaisseau."""

        if self._weapon_level == 1:
            self._limte = 1
            self._rayon = 90
            self._cooldown = 2000

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self._weapon_level == 2:
            self._cooldown = 1750
            self._rayon = 95
            self._limte = 2

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self._weapon_level == 3:
            self._cooldown = 1500
            self._rayon = 100
            self._limte = 3

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

    def active_invincibility(self):
        """la méthode active invincibility permet de rendre invincible le vaisseau du joueur pendant 15 secondes."""

        if self.invincible:
            self.invincible_2()
        else:
            self.invincible = True
            self._image = PhotoImage(file='SpaceShip/{}_invincible.gif'.format(self.type_space_ship))
            self._window.canva.itemconfigure(self._move_space_ship, image=self._image)

            self._window.after(15000, self.normal)

    def invincible_2(self):
        """La méthode invincible 2 permet de prolonger le temps d'invincibilité du vaisseau si il ressoit un nouveau bonus."""

        if self.invincible:
            self._window.after(50, self.invincible_2)
        else:
            self.active_invincibility()

    def normal(self):
        """La méthode normal permet d'enlever l'invincibilité du vaisseau."""

        self.invincible = False

        self._image = PhotoImage(file='SpaceShip/{}.gif'.format(self.type_space_ship))
        self._window.canva.itemconfigure(self._move_space_ship, image=self._image)

    def upgrad_max(self):
        """La méthode upgrade max permet d'améliorer les types de tir du vaisseau au niveau max."""

        if self.type_space_ship == 'player_laser':
            self._weapon_level = 3
            self._limte = 7
            self._cooldown = 500

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self.type_space_ship == 'player_plazma':
            self._weapon_level = 3
            self._cooldown = 200
            self._limte = 5

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self.type_space_ship == 'player_devastator':
            self._weapon_level = 3
            self._cooldown = 3000

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))

        elif self.type_space_ship == 'player_bomber':
            self._weapon_level = 3
            self._cooldown = 2000
            self._rayon = 100
            self._limte = 3

            self._window.canva.itemconfigure(self.print_level, text='Weapon level: {}'.format(self._weapon_level))


class Shot():
    """La classe Shot va permetre de creer, stocker, gerer les colisions,
        et faire bouger les différents tir du joueur mais aussi ceux des ennemies."""

    def __init__(self, window, coord, num, tag):

        if tag[0] == 'player':
            file = 'Laser/{}.png'.format(tag[-1])
            if tag[-1] == 'plazma':
                self._vitesse = 20
                temp = -35
            elif tag[-1] == 'devastator':
                self._vitesse = 25
                temp = -700
            elif tag[-1] == 'bomber':
                self._vitesse = 30
                temp = -40
            else:
                self._vitesse = 5
                temp = -35
        elif tag[0] == 'bot':
            file = 'Laser/laser.png'
            temp = 35
            self._vitesse = 5


        self._image = PhotoImage(file=file)
        self._coord = coord
        self._window = window
        self._num = num
        self._tag = tag
        self._move_shot = self._window.canva.create_image(self._coord[0], self._coord[-1]+temp, image=self._image)
        self.can_hit = True
        self.t = 0


    def shot_moving(self):
        "La méthode shot moving permet de faire bouger les tirs ainsi que de les supprimer si il sorte de l'écran."

        self._coord = self._window.canva.coords(self._move_shot)

        if self._coord != []:
            if self._tag[0] == 'player':
                if self._tag[-1] == 'devastator':
                    self._window.after(500, self.end_devastator)

                    self._window.canva.move(self._move_shot, 0, -5)
                    self._window.after(self._vitesse, self.shot_moving)

                elif self._coord[-1] < -10:
                    try:
                        del self._window.move_shot[self._num]
                    except:
                        pass
                    self._window.canva.delete(self._move_shot)

                else:
                    self._window.canva.move(self._move_shot, 0, -5)
                    self._window.after(self._vitesse, self.shot_moving)

            elif self._tag[0] == 'bot':

                if self._coord[-1] > 1090:
                    try:
                        del self._window.bot_shot[self._num]
                    except:
                        pass
                    self._window.canva.delete(self._move_shot)
                else:
                    self._window.canva.move(self._move_shot, 0, 5)
                    self._window.after(self._vitesse, self.shot_moving)

            self.colision()

    def colision(self):
        """La méthode colision permet de gérer les colisions entre les tirs et les différents objets du jeu."""

        if self._tag[-1] == 'devastator':
            nb_colision = self._window.canva.find_overlapping(self._coord[0]-25, self._coord[-1] -800, self._coord[0] + 25, self._coord[-1] + 800)
        elif self._tag[-1] == 'bomber':
            nb_colision = self._window.canva.find_overlapping(self._coord[0]-20, self._coord[-1] -20, self._coord[0] + 20, self._coord[-1] + 20)
        else:
            nb_colision = self._window.canva.find_overlapping(self._coord[0]-5, self._coord[-1] -10, self._coord[0] + 5, self._coord[-1] +10)

        for iid in nb_colision:
            if 'bot' in self._window.canva.gettags(iid) and self._tag[0] == 'player':
                if self._tag[-1] == 'bomber':
                    nb_destruction = self._window.canva.find_overlapping(self._coord[0]-self._window.space_ship._rayon, self._coord[-1] -self._window.space_ship._rayon, self._coord[0] + self._window.space_ship._rayon, self._coord[-1] + self._window.space_ship._rayon)

                    self._window.explosion_bomber(self._coord)
                    try:
                        del self._window.move_shot[self._num]
                    except:
                        pass
                    self._window.canva.delete(self._move_shot)

                    for idd in nb_destruction:
                        if 'bot' in self._window.canva.gettags(idd) and self._tag[0] == 'player':
                            if self._window.is_sfx:
                                self._window.channel_player.play(self._window.sound_seismic_charge)

                            self._coord = self._window.canva.coords(idd)
##                            self.destruction()

                            try:
                                del self._window.bot[int(self._window.canva.gettags(idd)[1])]
                            except:
                                pass

                            self._window.canva.delete(idd)

                            self._window.chose_power_up(self._coord)

                            self._window.score += 100

                            self._window.update_score(self._coord)

                else:
                    self._coord = self._window.canva.coords(iid)

                    if self._tag[-1] == 'laser' or self._tag[-1] == 'laser_blue':
                        self.destruction()

                    if self._tag[-1] == 'laser_blue' or self._tag[-1] == 'bomber':
                        try:
                            del self._window.move_shot[self._num]
                        except:
                            pass
                        self._window.canva.delete(self._move_shot)

                    if self._window.is_sfx:
                        self._window.sound_ennemi_explode.play()

                    try:
                        del self._window.bot[int(self._window.canva.gettags(iid)[1])]
                    except:
                        pass
                    self._window.canva.delete(iid)

                    self._window.chose_power_up(self._coord)

                    self._window.score += 100

                    self._window.update_score(self._coord)

                if self._window.bot == {}:
                    self._window.wave += 1
                    self._window.update_wave()
                    self._window.change_wave()

            elif 'player' in self._window.canva.gettags(iid) and self._tag[0] == 'bot' and self._window.space_ship is not None:
                self._coord = self._window.canva.coords(iid)

                self.destruction()

                try:
                    del self._window.bot_shot[self._num]
                except:
                    pass
                self._window.canva.delete(self._move_shot)

                if self._window.space_ship._shield > 0 and not self._window.space_ship.invincible:
                    self._window.space_ship._shield -= 1
                elif self._window.space_ship._health > 0 and not self._window.space_ship.invincible:
                    self._window.space_ship._health -= 1

                self._window.space_ship.health()

            elif 'meteor' in self._window.canva.gettags(iid) and self._tag[0] == 'player':
                if self._tag[-1] != 'plazma' and self._tag[-1] != 'devastator':
                    try:
                        del self._window.move_shot[self._num]
                    except:
                        pass
                    self._window.canva.delete(self._move_shot)

                    num = int(self._window.canva.gettags(iid)[1])
                    self._window.meteor[num]._health -= 1

                    self._window.meteor[num].destruction()
                    if self._tag[-1] == 'bomber':
                        self._window.explosion_bomber(self._coord)
                    else:
                        self.destruction(100)


                elif self.can_hit:
                    self.can_hit = False
                    self._window.after(50, self.hit)

                    num = int(self._window.canva.gettags(iid)[1])
                    self._window.meteor[num]._health -= 1

                    self._window.meteor[num].destruction()
##                    self.destruction(100)

    def end_devastator(self):
        """La méthode end devastator permet de mettre fin au laser du vaisseau en type de tir devastator."""

        try:
            del self._window.move_shot[self._num]
        except:
            pass
        self._window.canva.delete(self._move_shot)

    def hit(self):
        """La méthode hit permet a l'ennemi d'être touché."""
        self.can_hit = True

    def destruction(self, time=200):
        """La méthode destruction permet d'afficher une explosion la où il y a eut une colision entre un tir et un objet du jeu."""

        self._move_explosion = self._window.canva.create_image(self._coord[0], self._coord[-1], image=self._window.explosion_image)

        self._window.after(time, self.end)

    def end(self):
        """La méthode end permet d'enlever l'image de l'explosion due à une colision entre un tir et un objet du jeu."""

        self._window.canva.delete(self._move_explosion)


class Bot():
    """La classe bot permet de creer les ennemies due jeu.
        Elle permet de gérer leur mouvement, leurs actions, leur colisions, leur tir et leur destruction."""

    def __init__(self, window, coord, pixel, num):

        self._window = window
        self._coord = coord
        self._pixel = pixel
        self._first = False
        self._num= num

        self._image = PhotoImage(file='enemy.gif')
        self._move_bot = self._window.canva.create_image(self._coord[0], self._coord[-1], image=self._image, tags=['bot', self._num])

        self.shooting()

    def bot_moving_1(self):
        """La méthode bot moving 1 permet de faire bouger les ennemies selon un certain paterne."""

        self._coord = self._window.canva.coords(self._move_bot)

        if self._coord != []:
            if self._coord[0] < 25 or self._coord[0] > 1895:
                if self._first:
                    self._pixel = -self._pixel

            if 25 <= self._coord[0] <= 1895:
                self._first = True

            self._window.canva.move(self._move_bot, self._pixel, 0)
            self._window.after(40, self.bot_moving_1)

        self.colision()

    def bot_moving_2(self, limit, side):
        """La méthode bot moving 2 permet de faire bouger les ennemies selon un certain paterne."""

        self._coord = self._window.canva.coords(self._move_bot)

        if self._coord != []:
            if side:
                if self._coord[-1] == limit:
                    self._pixel = -5
                    self.bot_moving_1()
                else:
                    self._window.canva.move(self._move_bot, 0, self._pixel)
                    self._window.after(40, self.bot_moving_2, limit, True)
            else:
                if self._coord[-1] == limit:
                    self.bot_moving_1()
                else:
                    self._window.canva.move(self._move_bot, 0, self._pixel)
                    self._window.after(40, self.bot_moving_2, limit, False)

            self.colision()

    def shooting(self):
        """la méthode permet de faire tirer aléatoirement les ennemies.
            Leur fréquence de tir augement en fonction du nombre de vague effectué."""

        liste = [1, 2, 3, 4, 5]

        number = random.choice(liste)

        if number == 1:
            if self._coord != []:
                if self._window.is_sfx:
                    self._window.channel_ennemi.play(self._window.sound_laser)

                shot = Shot(self._window, self._coord, self._window.compteur_shoot_bot, ['bot'])

                self._window.bot_shot[self._window.compteur_shoot_bot] = shot

                shot.shot_moving()

                self._window.compteur_shoot_bot += 1

        time = 500+(500-self._window.wave*50)

        if time < 500:
            time = 500

        self._window.after(time, self.shooting)

    def colision(self):
        """la méthode colision permet de gérer la colision entre un ennemi et le vaisseau du joueur."""

        if self._coord != []:
            nb_colision = self._window.canva.find_overlapping(self._coord[0]-25, self._coord[-1] -25, self._coord[0] + 25, self._coord[-1] +25)

            for iid in nb_colision:
                if 'player' in self._window.canva.gettags(iid):

                    try:
                        del self._window.bot[self._num]
                    except:
                        pass
                    self._window.canva.delete(self._move_bot)

                    if self._window.space_ship._shield > 0 and not self._window.space_ship.invincible:
                        self._window.space_ship._shield -= 1

                    elif self._window.space_ship._health > 0 and not self._window.space_ship.invincible:
                        self._window.space_ship._health -= 1

                    self._window.space_ship.health()

                    self.destruction()

    def destruction(self, time=200):
        """la méthode destruction permet d'afficher une image d'explosion à l'endroit où a été détruit un ennemi."""

        self._move_explosion = self._window.canva.create_image(self._coord[0], self._coord[-1], image=self._window.explosion_image)

        self._window.after(time, self.end)

    def end(self):
        """la méthode permet d'enlever l'image d'explosion due à la destruction d'un ennemi."""

        self._window.canva.delete(self._move_explosion)


class Meteor():
    """La classe Meteor permet de creer les météroites du jeu.
        Elle permet de gérer leur mouvement, colisions et explosion."""

    def __init__(self, window, num):

        self._window = window
        self._num = num

        self._coord = [random.randint(9, 1912), -50]
        self.type_meteor = random.randint(0, 6)
        self._health = self.type_meteor + 1
        self._image = PhotoImage(file='Meteor/meteor_{}.png'.format(self.type_meteor))

        self._move_meteor = self._window.canva.create_image(self._coord[0], self._coord[-1], image=self._image, tags=['meteor', self._num])

        self.meteor_moving()

    def meteor_moving(self):
        """la méthode meteor moving permet de déplacer les météroites."""

        self._coord = self._window.canva.coords(self._move_meteor)
        if self._coord != []:
            if self._coord[-1] > 1150:
                try:
                    del self._window.meteor[self._num]
                except:
                    pass
                self._window.canva.delete(self._move_meteor)


            self.colision()
            self._window.canva.move(self._move_meteor, 0, 15)
            self._window.after(30, self.meteor_moving)

    def colision(self):
        """La méthode colision permet de gérer les colisions entre une météroite et le vaisseau d e joueur."""

        nb_colision = self._window.canva.find_overlapping(self._coord[0]-5, self._coord[-1] -10, self._coord[0] + 5, self._coord[-1] +10)

        for iid in nb_colision:
            if 'player' in self._window.canva.gettags(iid):
                if self._health <= 4:

                    if self._window.space_ship._shield > 0 and not self._window.space_ship.invincible:
                        self._window.space_ship._shield -= 1

                    elif self._window.space_ship._health > 0 and not self._window.space_ship.invincible:
                        self._window.space_ship._health -= 1

                    self._health = 0

                    self._window.space_ship.health()

                    self.destruction()

                else:
                    if not self._window.space_ship.invincible:
                        self._window.space_ship._health = 0
                        self._window.space_ship.health()
                    self._health = 0
                    self.destruction()

    def destruction(self):
        """la méthode permet de détruire une météroite quand sa vie est tombé à 0."""

        if self._health == 0:

            self._window.score += (self.type_meteor+1)*100
            self._window.update_score()
            self._window.canva.delete(self._move_meteor)
            self._window.explosion(self._coord)

            self._window.chose_power_up(self._coord)

            self._window.after(1000, self.end)

    def end(self):
        """La méthode permet de supprimer la météroite du jeu une fois détruit."""

        del self._window.meteor[self._num]


class PowerUp():
    """La classe PowerUp permet de créer les différents bonusdu jeu.
        Elle permet de gérer leur mouvement, leur colision et leur effet en cas de colision."""

    def __init__(self, window, tag, coord, num):

        self._window = window
        self._coord = coord
        self._num = num
        self._tag = tag

        self._image = PhotoImage(file='Power_up/{}.png'.format(tag))

        self.power_up = self._window.canva.create_image(self._coord[0], self._coord[-1], image=self._image, tags=tag)

        self.power_moving()

    def power_moving(self):
        """La méthode power moving permet de déplacer le bonus."""

        self._coord = self._window.canva.coords(self.power_up)

        if self._coord != []:
            if self._coord[-1] > 1090:
                try:
                    del self._window.power_up[self._num]
                except:
                    pass
                self._window.canva.delete(self.power_up)

            else:
                self._window.canva.move(self.power_up, 0, 10)

                self.colision()

                self._window.after(40, self.power_moving)

    def colision(self):
        """La méthode colision permet de gérer la colision entre un joueur et un bonus et de lancer son effet sur le vaisseau du joueur."""

        nb_colision = self._window.canva.find_overlapping(self._coord[0]-15, self._coord[-1] - 15, self._coord[0] + 15, self._coord[-1] + 15)

        for iid in nb_colision:
            if 'player' in self._window.canva.gettags(iid):
                try:
                    del self._window.power_up[self._num]
                except:
                    pass
                self._window.canva.delete(self.power_up)


                if self._tag == 'heal':
                    self._window.score += 500

                    if self._window.space_ship._health < 3:
                        self._window.space_ship._health += 1
                        self._window.space_ship.health()
                    else:
                        self._window.score += 500

                    self._window.update_score(self._coord)

                elif self._tag == 'full_heal':
                    if self._window.space_ship._health == 3:
                        self._window.score += 1000

                    self._window.score += 1000

                    self._window.update_score(self._coord)


                    self._window.space_ship._health = 3
                    self._window.space_ship.health()

                elif self._tag == 'shield':
                    self._window.score += 500


                    if self._window.space_ship._shield < 3:
                        self._window.space_ship._shield += 1
                        self._window.space_ship.health()
                    else:
                        self._window.score += 500

                    self._window.update_score(self._coord)

                elif self._tag == 'full_shield':
                    if self._window.space_ship._shield == 3:
                        self._window.score += 1000

                    self._window.score += 1000
                    self._window.update_score(self._coord)

                    self._window.space_ship._shield = 3
                    self._window.space_ship.health()

                elif self._tag == 'plazma':
                    self._window.score += 2000
                    self._window.update_score(self._coord)

                    self._window.space_ship.plazma()

                elif self._tag == 'devastator':
                    self._window.score += 2000
                    self._window.update_score(self._coord)

                    self._window.space_ship.devastator()

                elif self._tag == 'laser':
                    self._window.score += 2000
                    self._window.update_score(self._coord)

                    self._window.space_ship.laser()

                elif self._tag == 'bomber':
                    self._window.score += 2000
                    self._window.update_score(self._coord)

                    self._window.space_ship.bomber()

                elif self._tag == 'mega_heal':
                    self._window.score += 3000

                    if self._window.space_ship._health == 3:
                        self._window.score += 1000

                    else:
                        self._window.space_ship._health = 3
                        self._window.space_ship.health()

                    if self._window.space_ship._shield == 3:
                        self._window.score += 1000
                    else:
                        self._window.space_ship._shield = 3
                        self._window.space_ship.health()

                    self._window.update_score(self._coord)

                elif self._tag == 'invincibility':
                    self._window.score += 1500

                    self._window.space_ship.active_invincibility()

                elif self._tag == 'unlimited_power':
                    self._window.score += 10000
                    self._window.update_score(self._coord)

                    self._window.space_ship._health = 3
                    self._window.space_ship._shield = 3
                    self._window.space_ship.health()

                    self._window.space_ship.upgrad_max()

                    self._window.space_ship.active_invincibility()


class Timer():
    """La classe Timer permet de creer un chronomètre utilisé durant la partie.
        Elle permet de grérer le chronometre, de le lancer, de l'arrêter,
        de l'afficher et de le mettre à jour."""

    def __init__(self, window, **kw):
        self._start = 0.0
        self._window = window
        self._elapsedtime = 0.0
        self._running = False
        self.timestr = StringVar()
        self.makeWidgets()
        self.print_time = self._window.canva.create_text(self._window.winfo_screenwidth()-80, 30, text=self.timestr.get(), fill='white', font=('Times New Roman', 30), justify='left')

    def makeWidgets(self):
        """La méthode makewidget permet de créer le timer"""
        self._setTime(self._elapsedtime)

    def _update(self):
        """La méthode update permet de mettre à jour l'affichage en continue la valeur du chronomètre."""

        if self._running:
            self._elapsedtime = time.time() - self._start
            self._setTime(self._elapsedtime)
            self._window.canva.itemconfigure(self.print_time, text=self.timestr.get())
            self._timer = self._window.after(50, self._update)

    def _setTime(self, elap):
        """La méthode set time permet de mettre a jour en continue le temps du chronomètre."""

        minutes = int(elap/60)
        seconds = int(elap - minutes*60.0)
        hseconds = int((elap - minutes*60.0 - seconds)*100)
        self.timestr.set('%02d:%02d:%02d' % (minutes, seconds, hseconds))

    def Start(self):
        """La méthode Start permet de lancer le chronomètre."""

        if not self._running:
            self._start = time.time() - self._elapsedtime
            self._running = True
            self._update()

    def Stop(self):
        """La méthode Stop permet d'arreter le chronomètre."""

        if self._running:
##            self.after_cancel(self._timer)
            self._elapsedtime = time.time() - self._start
            self._setTime(self._elapsedtime)
            self._running = False

    def Reset(self):
        "La méthode Reset permet de réinitialiser le chronomètre."

        self._start = time.time()
        self._elapsedtime = 0.0
        self._setTime(self._elapsedtime)


game= SpaceInvader()
game.mainloop()