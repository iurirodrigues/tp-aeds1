#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

//variaveis globais
const float FPS = 30; //numero de quadros por segundo, a ser carregado no temporizador (altere esse valor e veja o que acontece)
const int SCREEN_W = 800;  //largura da tela
const int SCREEN_H = 600;  //altura da tela

int random(int min, int max) {
    return min + rand()%(max+1-min);
}

int main(int argc, char **argv){
    int score = 50;


    srand(time(NULL));
    //tela
    ALLEGRO_DISPLAY *display = NULL;
    //fila de eventos detectados pelo Allegro (ex: tecla que foi apertada, clique do mouse etc)
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    //temporizador: quando FPS = 10, a cada 0.1 segundos o tempo passa de t para t+1 e a fila de eventos detecta
    ALLEGRO_TIMER *timer = NULL;
    // Notas musicais
    ALLEGRO_BITMAP *nota11 = NULL;
    ALLEGRO_BITMAP *nota12 = NULL;
    ALLEGRO_BITMAP *nota21 = NULL;
    ALLEGRO_BITMAP *nota22 = NULL;
    ALLEGRO_BITMAP *nota31 = NULL;
    ALLEGRO_BITMAP *nota32 = NULL;
    ALLEGRO_BITMAP *nota41 = NULL;
    ALLEGRO_BITMAP *nota42 = NULL;
    
    //variavel que indica se eh para redesenhar o passaro
    bool redraw = true;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_SAMPLE *sample = NULL;
 
    //------------------------------ rotinas de inicializacao -------------------------

	//inicializa o allegro. Se nao conseguir, imprime na tela uma msg de erro.
    if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
    }

	//inicializa o temporizador com a taxa de 1 quadro a cada 0.1 segundos. Se nao conseguir, imprime na tela uma msg de erro.
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
    }

	//inicializa a tela. Se nao conseguir, imprime na tela uma msg de erro.
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
    }

    al_init_image_addon();


    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        return -1;
    }
    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return -1;
    }
    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }

    // Inicializa a Música
    sample = al_load_sample("theme.ogg");
    if (!sample){
        printf( "Audio clip sample not loaded!\n" ); 
        return -1;
    }

    // Inicializa a Imagem
    imagem = al_load_bitmap("bg.png");
    if (!imagem){
        printf( "imagem sample not loaded!\n" ); 
        return -1;
    }
    al_draw_bitmap(imagem, 0, 0, 0);


    // Teclado
    al_install_keyboard();

// ---------- fontes
    //inicializa o modulo allegro que carrega as fontes
    al_init_font_addon();
	//inicializa o modulo allegro que entende arquivos tff de fontes
    al_init_ttf_addon();

	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   	


    //variacao de x e y ao longo do tempo
    float nota_dy = 5.0;


    // ----------- Nota 1
    nota11 = al_load_bitmap("nota1.png");
    float nota11_x = SCREEN_W / 2.0 - 50 - 19 - 50 - 7;
    float nota11_y = random(50, 500) * -1;

    nota12 = al_load_bitmap("nota1.png");
    float nota12_x = SCREEN_W / 2.0 - 50 - 19 - 50 - 7;
    float nota12_y = random(60, 500) * -1;

    // -- nota 2
    nota21 = al_load_bitmap("nota2.png");
    float nota21_x = SCREEN_W / 2.0 - 50 - 10;
    float nota21_y = random(50, 500) * -1;

    nota22 = al_load_bitmap("nota2.png");
    float nota22_x = SCREEN_W / 2.0 - 50 - 10;
    float nota22_y = random(60, 500) * -1;

    // -- nota 3
    nota31 = al_load_bitmap("nota3.png");
    float nota31_x = SCREEN_W / 2.0 + 7;
    float nota31_y = random(50, 500) * -1;

    nota32 = al_load_bitmap("nota3.png");
    float nota32_x = SCREEN_W / 2.0 + 7;
    float nota32_y = random(60, 500) * -1;

    // -- nota 4
    nota41 = al_load_bitmap("nota4.png");
    float nota41_x = SCREEN_W / 2.0 + 76;
    float nota41_y = random(50, 500) * -1;

    nota42 = al_load_bitmap("nota4.png");
    float nota42_x = SCREEN_W / 2.0 + 76;
    float nota42_y = random(60, 500) * -1;


    //criar a fila de eventos
    event_queue = al_create_event_queue();
	//se nao conseguiu criar a fila de eventos
    if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
    }

    //registrar mudancas na tela dentro da fila de eventos, isto e, sempre que a tela mudar, um evento ocorrerah
    al_register_event_source(event_queue, al_get_display_event_source(display));

    //coloca o timer na fila de eventos, isto e, sempre que o tempo passar, um evento eh gerado
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    
    // teclado
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    //reinicializa a tela
    al_flip_display();

    //inicia o temporizador


    al_start_timer(timer);

    int tTimer = 0;
    //enquanto a posicao x do passaro for menor que a largura da tela
    while(tTimer < 74 && score > -100) {
        al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        tTimer = al_get_timer_count(timer)/(int)FPS;


        
        //variavel do tipo evento
        ALLEGRO_EVENT ev;
        //a variavel ev recebe o primeiro evento da fila de eventos
        al_wait_for_event(event_queue, &ev);
        //se for um evento de timer, ou seja, se foi o tempo que passou de t para t+1
        
        if(nota11_y > 600) {
            nota11_y = random(50, 500) * -1;
            score -= 10;
        }
        if(nota12_y > 600) {
            nota12_y = random(60, 500) * -1;
            score -= 10;
        }
        if(nota21_y > 600) {
            nota21_y = random(50, 500) * -1;
            score -= 10;
        }       
        if(nota22_y > 600) {
            nota22_y = random(60, 500) * -1;
            score -= 10;
        }
        if(nota31_y > 600) {
            nota31_y = random(50, 500) * -1;
            score -= 10;
        }
        if(nota32_y > 600) {
            nota32_y = random(60, 500) * -1;
            score -= 10;
        }
        if(nota41_y > 600) {
            nota41_y = random(50, 500) * -1;
            score -= 10;
        }
        if(nota42_y > 600) {
            nota42_y = random(60, 500) * -1;
            score -= 10;
        }
        

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_Q:
                    if(nota11_y >= 500 && nota11_y <= 600) {
                        printf("\a");
                        nota11_y = random(50, 500) * -1;
                        score += 5;
                    }
                    else if(nota12_y >= 500 && nota12_y <= 600) {
                        printf("\a");
                        nota12_y = random(50, 500) * -1;
                        score += 5;
                    }
                    else score -= 10;
                    break;

                case ALLEGRO_KEY_W:
                    if(nota21_y >= 500 && nota21_y <= 600) {
                        printf("\a");
                        nota21_y = random(50, 500) * -1;
                        score += 5;
                    }
                    else if(nota22_y >= 500 && nota22_y <= 600) {
                        printf("\a");
                        nota22_y = random(50, 500) * -1;
                        score += 5;
                    }
                    else score -= 10;
                    break;

                case ALLEGRO_KEY_O:
                    if(nota31_y >= 500 && nota31_y <= 600) {
                        printf("\a");
                        nota31_y = random(50, 500) * -1;
                        score += 5;
                    }
                    else if(nota32_y >= 500 && nota32_y <= 600) {
                        printf("\a");
                        nota32_y = random(50, 500) * -1;
                        score += 5;
                    }
                    else score -= 10;
                    break;

                case ALLEGRO_KEY_P:    
                    if(nota41_y >= 500 && nota41_y <= 600) {
                        printf("\a");
                        nota41_y = random(50, 500) * -1;
                        score += 5;
                    }
                    else if(nota42_y >= 500 && nota42_y <= 600) {
                        printf("\a");
                        nota42_y = random(50, 500) * -1;
                        score += 5;
                    }
                    else score -= 10;
                    break;
            }
        }
        
        if(ev.type == ALLEGRO_EVENT_TIMER) {
 
           
            //incrementa as posicoes x e y do passaro com o seu deslocamento dx e dy
            nota11_y += nota_dy;
            nota12_y += nota_dy;
            nota21_y += nota_dy;
            nota22_y += nota_dy;
            nota31_y += nota_dy;
            nota32_y += nota_dy;
            nota41_y += nota_dy;
            nota42_y += nota_dy;
 
            //como eu movi o passaro, preciso redesenhar ele (remova essa linha e veja o que acontece)
            redraw = true;

           
        }
        //se o evento for o fechamento da tela (clicando no x no canto superior direito)
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            //paro de executar o while
            break;
        }
        //se for para eu redesenhar a tela e nao tenho mais nenhum evento para ler
        if(redraw && al_is_event_queue_empty(event_queue)) {
            //nao preciso redesenhar
            redraw = false;
            //limpo a tela
            al_draw_bitmap(imagem, 0, 0, 0);    
            //desenho o passaro na nova posicao
            al_draw_bitmap(nota11, nota11_x, nota11_y, 0);
            al_draw_bitmap(nota12, nota12_x, nota12_y, 0);
            al_draw_bitmap(nota21, nota21_x, nota21_y, 0);
            al_draw_bitmap(nota22, nota22_x, nota22_y, 0);
            al_draw_bitmap(nota31, nota31_x, nota31_y, 0);
            al_draw_bitmap(nota32, nota32_x, nota32_y, 0);
            al_draw_bitmap(nota41, nota41_x, nota41_y, 0);
            al_draw_bitmap(nota42, nota42_x, nota42_y, 0);

            //dou um refresh na tela

            char my_text[20];
            sprintf(my_text, "Score: %d", score);
            al_draw_text(size_32, al_map_rgb(200, 0, 30), 50, SCREEN_H/2, 0, my_text);
            al_flip_display();


            char time[20];
            sprintf(time, "Tempo: %d", tTimer);
            al_draw_text(size_32, al_map_rgb(200, 0, 30), 50, SCREEN_H/2 + 50, 0, time);
            al_flip_display();
        }






    } //fim while

    //rotinas de fim de jogo
    al_destroy_sample(sample);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
