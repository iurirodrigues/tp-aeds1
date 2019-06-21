#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//variaveis globais
const float FPS = 50; //numero de quadros por segundo, a ser carregado no temporizador (altere esse valor e veja o que acontece)
const int SCREEN_W = 800;  //largura da tela
const int SCREEN_H = 600;  //altura da tela

int random(int min, int max) {
    return min + rand()%(max+1-min);
}

int main(int argc, char **argv){
    srand(time(NULL));
    //tela
    ALLEGRO_DISPLAY *display = NULL;
    //fila de eventos detectados pelo Allegro (ex: tecla que foi apertada, clique do mouse etc)
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    //temporizador: quando FPS = 10, a cada 0.1 segundos o tempo passa de t para t+1 e a fila de eventos detecta
    ALLEGRO_TIMER *timer = NULL;
    // Notas musicais
    ALLEGRO_BITMAP *nota1 = NULL;
    ALLEGRO_BITMAP *nota2 = NULL;
    
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
    al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

    // Inicializa a Imagem
    imagem = al_load_bitmap("bg.png");
    if (!imagem){
        printf( "imagem sample not loaded!\n" ); 
        return -1;
    }
    al_draw_bitmap(imagem, 0, 0, 0);


    // Teclado
    al_install_keyboard();


    // ----------- Nota 1
    nota1 = al_load_bitmap("nota1.png");
	//se nao conseguiu achar o arquivo, imprime uma msg de erro
    if(!nota1) {
      fprintf(stderr, "failed to create bird bitmap!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
    }
    //largura e altura da figura do passaro
    float largura_passaro =  al_get_bitmap_width(nota1);
    float altura_passaro =  al_get_bitmap_height(nota1);
    //posicao x e y inicial do passaro na tela
    float bird_x = SCREEN_W / 2.0 - 50 - 19 - 50 - 7;
    float bird_y = random(50, 500) * -1;

    //variacao de x e y ao longo do tempo
    float bird_dx = 0.0, bird_dy = 5.0;
    
    nota2 = al_load_bitmap("nota2.png");
    float nota2_x = SCREEN_W / 2.0 - 50 - 10;
    float nota2_y = random(50, 500) * -1;

    //criar a fila de eventos
    event_queue = al_create_event_queue();
	//se nao conseguiu criar a fila de eventos
    if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_bitmap(nota1);
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

    //enquanto a posicao x do passaro for menor que a largura da tela
    while(1==1)
    {
        //variavel do tipo evento
        ALLEGRO_EVENT ev;
        //a variavel ev recebe o primeiro evento da fila de eventos
        al_wait_for_event(event_queue, &ev);
        //se for um evento de timer, ou seja, se foi o tempo que passou de t para t+1
        
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_Q:
                    if(bird_y >= 500 && bird_y <= 600) {
                        printf("\a");
                        bird_y = random(50, 500) * -1;
                    }
                    break;

                case ALLEGRO_KEY_W:
                    if(nota2_y >= 500 && nota2_y <= 600) {
                        printf("\a");
                        nota2_y = random(50, 500) * -1;
                    }
                    break;
                case ALLEGRO_KEY_O:
                    break;
                case ALLEGRO_KEY_P:    
                    break;
            }
        }
        
        if(ev.type == ALLEGRO_EVENT_TIMER) {
 
           
            //incrementa as posicoes x e y do passaro com o seu deslocamento dx e dy
            bird_x += bird_dx;
            bird_y += bird_dy;

            nota2_x += bird_dx;
            nota2_y += bird_dy;
            
            if(bird_y >= 600) {
                bird_y = random(50, 500) * -1;
            }
            if(nota2_y >= 600) {
                nota2_y = random(50, 500) * -1;
            }
            
            //como eu movi o passaro, preciso redesenhar ele (remova essa linha e veja o que acontece)
            redraw = true;

           
        }
        //se o evento for o fechamento da tela (clicando no x no canto superior direito)
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE  ) {
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
            al_draw_bitmap(nota1, bird_x, bird_y, 0);
            al_draw_bitmap(nota2, nota2_x, nota2_y, 0);
            //dou um refresh na tela
            al_flip_display();
        }
    } //fim while

    //rotinas de fim de jogo
    al_destroy_sample(sample);
    al_destroy_bitmap(nota1);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
