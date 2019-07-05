#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// Variaveis globais
const float FPS = 40; // numero de quadros por segundo
const int SCREEN_W = 800;  // largura da tela
const int SCREEN_H = 600;  // altura da tela

int notasAleatorias(int min, int max) {
    return min + rand()%(max+1-min);
}

void moveFluffy(float *fluffy_y, int score) {
    if(score==0 && *fluffy_y < 480) {
        *fluffy_y += 10;
    }
    if(score==1 && *fluffy_y > 280) {
        *fluffy_y -= 5;
    }
}

void verificaNotas(float *nota1, float *nota2) {
    if(*nota2 >= *nota1 && *nota2 <= *nota1+50)
        *nota1 = *nota1 + 50 - *nota2;
    if(*nota1 >= *nota2 && *nota1 <= *nota2+50)
        *nota2 = *nota2 + 50 - *nota1;
}


// ---------------------------------------------- Main ---------------------------------------------- //
int main(int argc, char **argv){
    FILE *record;
    int high_score;
    int score = 0;
    int enter = 0;
    int tTimer = 0;
    srand(time(NULL));
    bool redraw = true;

    // ------------------- Declaração de váriaveis ------------------- //
    // Allegro
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    // Imagens do Jogo
    ALLEGRO_BITMAP *frame1 = NULL;
    ALLEGRO_BITMAP *frame2 = NULL;
    ALLEGRO_BITMAP *frame3 = NULL;
    ALLEGRO_BITMAP *frame4 = NULL;
    ALLEGRO_BITMAP *frame5 = NULL;
    ALLEGRO_BITMAP *frame6 = NULL;
    ALLEGRO_BITMAP *frame7 = NULL;
    ALLEGRO_BITMAP *frame8 = NULL;
    ALLEGRO_BITMAP *frame9 = NULL;
    ALLEGRO_BITMAP *frame10 = NULL;
    ALLEGRO_BITMAP *frame11 = NULL;
    ALLEGRO_BITMAP *fluffy = NULL;
    ALLEGRO_BITMAP *bg_jogo = NULL;
    ALLEGRO_BITMAP *bg_gameover = NULL;
    ALLEGRO_BITMAP *bg_ganhou = NULL;
    ALLEGRO_BITMAP *harry = NULL;
    ALLEGRO_BITMAP *rony = NULL;


    // Notas musicais
    ALLEGRO_BITMAP *nota11 = NULL;
    ALLEGRO_BITMAP *nota12 = NULL;
    ALLEGRO_BITMAP *nota21 = NULL;
    ALLEGRO_BITMAP *nota22 = NULL;
    ALLEGRO_BITMAP *nota31 = NULL;
    ALLEGRO_BITMAP *nota32 = NULL;
    ALLEGRO_BITMAP *nota41 = NULL;
    ALLEGRO_BITMAP *nota42 = NULL;

    // Sons
    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_SAMPLE *notaSound = NULL;
	ALLEGRO_SAMPLE_INSTANCE *som_nota = NULL;
    ALLEGRO_SAMPLE *coruja = NULL;
    ALLEGRO_SAMPLE *fluffysharp = NULL;
    ALLEGRO_SAMPLE *theme_ganhou = NULL;

    // ------------------- Rotinas de Inicialização ------------------- //
    if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
    }

    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize keyboard!\n");
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
    }

	display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
    }

    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        return -1;
    }

    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return -1;
    }
    
    if (!al_reserve_samples(10)){
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }

    if(!al_init_image_addon()){
        fprintf(stderr, "failed to initialize image codecs!\n");
        return -1;
    }
    al_init_font_addon();
    al_init_ttf_addon();

    // ------------------- Bitmaps ------------------- //
    frame1 = al_load_bitmap("resources//images//intro//frame1.png");
    frame2 = al_load_bitmap("resources//images//intro//frame2.png");
    frame3 = al_load_bitmap("resources//images//intro//frame3.png");
    frame4 = al_load_bitmap("resources//images//intro//frame4.png");
    frame5 = al_load_bitmap("resources//images//intro//frame5.png");
    frame6 = al_load_bitmap("resources//images//intro//frame6.png");
    frame7 = al_load_bitmap("resources//images//intro//frame7.png");
    frame8 = al_load_bitmap("resources//images//intro//frame8.png");
    frame9 = al_load_bitmap("resources//images//intro//frame9.png");
    frame10 = al_load_bitmap("resources//images//intro//frame11.png");
    
    bg_jogo = al_load_bitmap("resources//images//bg_jogo.png");
    fluffy = al_load_bitmap("resources//images//fluffy.png");
    float fluffy_y = 380;

    // Notas
    nota11 = al_load_bitmap("resources//images//nota1.png");
    float nota11_y = notasAleatorias(50, 500) * -1;
    nota12 = al_load_bitmap("resources//images//nota1.png");
    float nota12_y = notasAleatorias(60, 500) * -1;
    verificaNotas(&nota11_y, &nota12_y);
    nota21 = al_load_bitmap("resources//images//nota2.png");
    float nota21_y = notasAleatorias(50, 500) * -1;
    nota22 = al_load_bitmap("resources//images//nota2.png");
    float nota22_y = notasAleatorias(60, 500) * -1;
    verificaNotas(&nota21_y, &nota22_y);
    nota31 = al_load_bitmap("resources//images//nota3.png");
    float nota31_y = notasAleatorias(50, 500) * -1;
    nota32 = al_load_bitmap("resources//images//nota3.png");
    float nota32_y = notasAleatorias(60, 500) * -1;
    verificaNotas(&nota31_y, &nota32_y);
    nota41 = al_load_bitmap("resources//images//nota4.png");
    float nota41_y = notasAleatorias(50, 500) * -1;
    nota42 = al_load_bitmap("resources//images//nota4.png");
    float nota42_y = notasAleatorias(60, 500) * -1;
    verificaNotas(&nota41_y, &nota42_y);
    
    float nota_dy = 5.0;

    // ------------------- Sons ------------------- //
    sample = al_load_sample("resources//sounds//normal.ogg");
    fluffysharp = al_load_sample("resources//sounds//fluffysharp.ogg");
    coruja = al_load_sample("resources//sounds//coruja.ogg");

    notaSound = al_load_sample("resources//sounds//som_nota.ogg");
    som_nota = al_create_sample_instance(notaSound);
    al_set_sample_instance_playmode(som_nota, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(som_nota, al_get_default_mixer());

    // ------------------- Fontes ------------------- //
    ALLEGRO_FONT *CentSchbkCyrill = al_load_font("resources//CentSchbkCyrill.ttf", 50, 1);
    ALLEGRO_FONT *pixelmix= al_load_font("resources//pixelmix.ttf", 13, 1);

    // Arquivos
    record = fopen("resources//record.txt","r");
    fscanf(record, "%d", &high_score); //armazena conteudo do arquivo na variavel high_score
    fclose(record);

    //criar a fila de events
    event_queue = al_create_event_queue();

    //registrar mudancas na tela dentro da fila de eventos, isto e, sempre que a tela mudar, um evento ocorrerah
    al_register_event_source(event_queue, al_get_display_event_source(display));

    //coloca o timer na fila de eventos, isto e, sempre que o tempo passar, um evento eh gerado
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    
    // teclado
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // ABERTURA -----------------------------------------------------------
    al_play_sample(fluffysharp, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    
    al_draw_bitmap(frame1, 0, 0, 0);
    al_flip_display();
    al_rest(6.0);

    al_draw_bitmap(frame2, 0, 0, 0);
    al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    al_flip_display();
    al_rest(3.0);

    al_draw_bitmap(frame3, 0, 0, 0);
    al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    al_flip_display();
    al_rest(3.0);

    al_draw_bitmap(frame4, 0, 0, 0);
    al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    al_flip_display();
    al_rest(3.0);

    al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    while(enter==0) {
        al_draw_bitmap(frame5, 0, 0, 0);
        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) { 
            switch(ev.keyboard.keycode) {
                case(ALLEGRO_KEY_ENTER):
                enter = 1;
            }
        }
    }

    al_draw_bitmap(frame6, 0, 0, 0);
    al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    al_flip_display();
    al_rest(3.0);

    al_draw_bitmap(frame7, 0, 0, 0);
    al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    al_flip_display();
    al_rest(3.0);

    al_draw_bitmap(frame8, 0, 0, 0);
    al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    al_flip_display();
    al_rest(3.0);


    al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    while(enter==1) {
        al_draw_bitmap(frame9, 0, 0, 0);
        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) { 
            switch(ev.keyboard.keycode) {
                case(ALLEGRO_KEY_ENTER):
                enter = 0;
            }
        }      
    }

    while(enter==0) {
        al_draw_bitmap(frame10, 0, 0, 0);
        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) { 
            switch(ev.keyboard.keycode) {
                case(ALLEGRO_KEY_ENTER):
                    al_destroy_sample(fluffysharp);
                    enter = 1;
            }
        }
    }

    al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    // JOGO --------------------------------------------------------------
    while(tTimer < 94 && score > -100) {
        al_draw_bitmap(bg_jogo, 0, 0, 0);
        al_start_timer(timer);
        tTimer = al_get_timer_count(timer)/(int)FPS;
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        int verificaScore;

        // Perca de pontos caso ultrapasse a area crítica
        if(nota11_y > 600) {
            nota11_y = notasAleatorias(50, 500) * -1;
            score -= 10;
            verificaScore = 0;
        }
        if(nota12_y > 600) {
            nota12_y = notasAleatorias(60, 500) * -1;
            score -= 10;
            verificaScore = 0;

        }
        if(nota21_y > 600) {
            nota21_y = notasAleatorias(50, 500) * -1;
            score -= 10;
            verificaScore = 0;

        }       
        if(nota22_y > 600) {
            nota22_y = notasAleatorias(60, 500) * -1;
            score -= 10;
            verificaScore = 0;

        }
        if(nota31_y > 600) {
            nota31_y = notasAleatorias(50, 500) * -1;
            score -= 10;
            verificaScore = 0;

        }
        if(nota32_y > 600) {
            nota32_y = notasAleatorias(60, 500) * -1;
            score -= 10;
            verificaScore = 0;

        }
        if(nota41_y > 600) {
            nota41_y = notasAleatorias(50, 500) * -1;
            score -= 10;
            verificaScore = 0;

        }
        if(nota42_y > 600) {
            nota42_y = notasAleatorias(60, 500) * -1;
            score -= 10;
            verificaScore = 0;

        }

        // 
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_Q:
                    if(nota11_y >= 500 && nota11_y <= 600) {
                        al_stop_sample_instance(som_nota);
                        al_play_sample_instance(som_nota);
                        nota11_y = notasAleatorias(50, 500) * -1;
                        score += 5;      
                        verificaScore = 1;
                    }
                    else if(nota12_y >= 500 && nota12_y <= 600) {
                        al_stop_sample_instance(som_nota);
                        al_play_sample_instance(som_nota);
                        nota12_y = notasAleatorias(50, 500) * -1;
                        score += 5;
                        verificaScore = 1;
                    }
                    else {
                        score -= 10;
                        verificaScore = 0;

                    }
                    break;

                case ALLEGRO_KEY_W:
                    if(nota21_y >= 500 && nota21_y <= 600) {
                        al_stop_sample_instance(som_nota);
                        al_play_sample_instance(som_nota);
                        nota21_y = notasAleatorias(50, 500) * -1;
                        score += 5;
                        verificaScore = 1;

                    }
                    else if(nota22_y >= 500 && nota22_y <= 600) {
                        al_stop_sample_instance(som_nota);
                        al_play_sample_instance(som_nota);
                        nota22_y = notasAleatorias(50, 500) * -1;
                        score += 5;
                        verificaScore = 1;

                    }
                    else {
                        score -= 10;
                        verificaScore = 0;

                    }
                    break;

                case ALLEGRO_KEY_O:
                    if(nota31_y >= 500 && nota31_y <= 600) {
                        al_stop_sample_instance(som_nota);
                        al_play_sample_instance(som_nota);
                        nota31_y = notasAleatorias(50, 500) * -1;
                        score += 5;
                        verificaScore = 1;

                    }
                    else if(nota32_y >= 500 && nota32_y <= 600) {
                        al_stop_sample_instance(som_nota);
                        al_play_sample_instance(som_nota);
                        nota32_y = notasAleatorias(50, 500) * -1;
                        score += 5;
                        verificaScore = 1;

                    }
                    else {
                        score -= 10;
                        verificaScore = 0;

                    }
                    break;

                case ALLEGRO_KEY_P:    
                    if(nota41_y >= 500 && nota41_y <= 600) {
                        al_stop_sample_instance(som_nota);
                        al_play_sample_instance(som_nota);
                        nota41_y = notasAleatorias(50, 500) * -1;
                        score += 5;
                        verificaScore = 1;

                    }
                    else if(nota42_y >= 500 && nota42_y <= 600) {
                        al_stop_sample_instance(som_nota);
                        al_play_sample_instance(som_nota);
                        nota42_y = notasAleatorias(50, 500) * -1;
                        score += 5;
                        verificaScore = 1;

                    }
                    else {
                        score -= 10;
                        verificaScore = 0;

                    }
                    break;
            }
        }

        verificaNotas(&nota11_y, &nota12_y);
        verificaNotas(&nota21_y, &nota22_y);
        verificaNotas(&nota31_y, &nota32_y);
        verificaNotas(&nota41_y, &nota42_y);

        moveFluffy(&fluffy_y, verificaScore);
        verificaScore = 2;

        if(ev.type == ALLEGRO_EVENT_TIMER) {
            nota11_y += nota_dy;
            nota12_y += nota_dy;
            nota21_y += nota_dy;
            nota22_y += nota_dy;
            nota31_y += nota_dy;
            nota32_y += nota_dy;
            nota41_y += nota_dy;
            nota42_y += nota_dy;
            redraw = true;
        }

        if(redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_draw_bitmap(bg_jogo, 0, 0, 0);    
            al_draw_bitmap(nota11, 300, nota11_y, 0);
            al_draw_bitmap(nota12, 300, nota12_y, 0);
            al_draw_bitmap(nota21, 370, nota21_y, 0);
            al_draw_bitmap(nota22, 370, nota22_y, 0);
            al_draw_bitmap(nota31, 440, nota31_y, 0);
            al_draw_bitmap(nota32, 440, nota32_y, 0);
            al_draw_bitmap(nota41, 510, nota41_y, 0);
            al_draw_bitmap(nota42, 510, nota42_y, 0);
            al_draw_bitmap(fluffy, 85, fluffy_y, 0);

            char my_text[10];
            sprintf(my_text, "%d", score);
            al_draw_text(CentSchbkCyrill, al_map_rgb(254, 254, 254), 120, 130, ALLEGRO_ALIGN_CENTRE, my_text);

            al_flip_display();
        }
    } 
    al_destroy_sample(sample);

    if(score > high_score) {
        record = fopen("resources//record.txt", "w"); //o arquivo eh agora aberto para escrita
        fprintf(record, "%d", score); //escreve no arquivo o novo recorde alcançado
        fclose(record);
        high_score = score;
    }

    coruja = al_load_sample("resources//sounds//coruja.ogg");
    theme_ganhou = al_load_sample("resources//sounds//ganhou.ogg");
    fluffysharp = al_load_sample("resources//sounds//fluffysharp.ogg");
    

    // Final
    if(score <= -100) {
        al_play_sample(fluffysharp, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        char hermionetext[100];
        sprintf(hermionetext, "Ah, nao! Voce acordou o Fluffy, Harry!");
        bg_gameover = al_load_bitmap("resources//images//bg_gameover.png");
        al_draw_bitmap(bg_gameover, 0, 0, 0);
        al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        al_draw_text(pixelmix, al_map_rgb(254, 254, 254), 245, 400, 0, hermionetext);
        al_flip_display();
        al_rest(3.0);

        char ronytext[100];
        sprintf(ronytext, "Voce conseguiu %d pontos!", score);
        rony = al_load_bitmap("resources//images//rony.png");
        al_draw_bitmap(rony, 64, 435, 0);
        al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        al_draw_text(pixelmix, al_map_rgb(254, 254, 254), 345, 468, ALLEGRO_ALIGN_CENTRE, ronytext);
        al_flip_display();
        al_rest(3.0);

        char harrytext[100];
        sprintf(harrytext, "Poxa! Meu record eh %d pontos!", high_score);
        harry = al_load_bitmap("resources//images//harry.png");
        al_draw_bitmap(harry, 217, 500, 0);
        al_play_sample(coruja, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        al_draw_text(pixelmix, al_map_rgb(254, 254, 254), 415, 533, ALLEGRO_ALIGN_CENTRE, harrytext);
        al_flip_display();
        al_rest(10.0);
    }
    else {
        al_play_sample(theme_ganhou, 0.8, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
        char hermionetext[100];
        sprintf(hermionetext, "YAYYY! CONSEGUIMOS PASSAR PELO FLUFFY!");
        bg_ganhou = al_load_bitmap("resources//images//bg_ganhou.png");
        al_draw_bitmap(bg_ganhou, 0, 0, 0);
        al_play_sample(coruja, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        al_draw_text(pixelmix, al_map_rgb(254, 254, 254), 245, 400, 0, hermionetext);
        al_flip_display();
        al_rest(3.0);

        char ronytext[100];
        sprintf(ronytext, "Voce conseguiu %d pontos!", score);
        rony = al_load_bitmap("resources//images//rony.png");
        al_draw_bitmap(rony, 64, 435, 0);
        al_play_sample(coruja, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        al_draw_text(pixelmix, al_map_rgb(254, 254, 254), 345, 468, ALLEGRO_ALIGN_CENTRE, ronytext);
        al_flip_display();
        al_rest(3.0);

        char harrytext[100];
        sprintf(harrytext, "Meu record eh %d pontos!", high_score);
        harry = al_load_bitmap("resources//images//harry.png");
        al_draw_bitmap(harry, 217, 500, 0);
        al_play_sample(coruja, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        al_draw_text(pixelmix, al_map_rgb(254, 254, 254), 415, 533, ALLEGRO_ALIGN_CENTRE, harrytext);
        al_flip_display();
        al_rest(10.0);
    }

    //rotinas de fim de jogo
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);


    return 0;
}
