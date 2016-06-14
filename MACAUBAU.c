#include<allegro.h>
#include<stdlib.h>
#include<time.h>

volatile int saida_programa = FALSE;
void fechar_X()
{
 saida_programa = TRUE;
}
END_OF_FUNCTION(fechar_X);


volatile int tempMsec;
void tempo_Msec() {tempMsec++;}
END_OF_FUNCTION(tempo_bomba);


/// *=== Prototipos ===* ///
void func_Colisao(int,float,float,int*);
void func_Nivel(int,float*,int*);
void mov_Personagem(int*,int*,int*,int*,int*,int*);

/// === Struct da Turtle === ///

  struct turtle
 {
  float yTurtle;
  float xTurtle;
  float angulo;
  int tempo;
 };


int main()
{
 allegro_init();
 install_timer();
 install_keyboard();
 set_color_depth(32);
 set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
 set_window_title("MACAUBAU");

 /// Funcoes
 LOCK_VARIABLE(saida_programa);
 LOCK_FUNCTION(fechar_X);
 set_close_button_callback(fechar_X);


 tempMsec=0;
 LOCK_FUNCTION(tempo_Turtle);
 LOCK_VARIABLE(tempMsec);
 install_int_ex(tempo_Msec, MSEC_TO_TIMER(1));

 int exit_program = FALSE;

 /// BITMAPS
 BITMAP* buffer=create_bitmap(SCREEN_W, SCREEN_H);
 BITMAP* personagem=load_bitmap("imagens/m1d.bmp", NULL);
 BITMAP* fundo=load_bitmap("imagens/fundo.bmp",NULL);
 BITMAP* turtle=load_bitmap("imagens/t1.bmp", NULL);
 BITMAP* gameover=load_bitmap("imagens/gameover.bmp",NULL);
 BITMAP* fail=load_bitmap("imagens/fail.bmp",NULL);

 BITMAP* per[4];
 per[0]=load_bitmap("imagens/m1d.bmp",NULL);
 per[1]=load_bitmap("imagens/m2d.bmp",NULL);
 per[2]=load_bitmap("imagens/m1e.bmp",NULL);
 per[3]=load_bitmap("imagens/m2e.bmp",NULL);

 /// Fonts
 FONT * fontscore = load_font("fontscore.pcx",NULL,NULL);

 /// Variaveis
 int score = 0,printnv=1;
 float nivel=1;
 int iPersonagem=0, seed, xPersonagem=0, colisao=0;
 int tempo_mov=tempMsec, tempo_animacao=tempMsec;
 int finalScore=0;

 struct turtle vetTurtle[4]={0,0,0,0};

 vetTurtle[0].tempo=tempMsec;
 vetTurtle[1].tempo=tempMsec;
 vetTurtle[2].tempo=tempMsec;

 while(!saida_programa)
 {
   /// ENTRADA
   if (key[KEY_ESC]) saida_programa=TRUE;

   /// ATUALIZACAO DE DADOS
   if(tempMsec-vetTurtle[0].tempo>=20)
   {
    vetTurtle[0].yTurtle+=10*nivel;
    vetTurtle[0].angulo+=7;
    vetTurtle[0].tempo=tempMsec;
   }

   if(vetTurtle[0].yTurtle>560 && colisao!=1)
   {
    seed=time(0);
    srand(seed);
    vetTurtle[0].xTurtle=(rand()%13)*70;

    score+=1;
    vetTurtle[0].yTurtle=0;
   }


   if(tempMsec-vetTurtle[1].tempo>=20)
   {
    vetTurtle[1].yTurtle+=9*nivel;
    vetTurtle[1].angulo+=7;
    vetTurtle[1].tempo=tempMsec;
   }

   if(vetTurtle[1].yTurtle>560 && colisao!=1)
   {
    seed=time(0);
    srand(seed);
    vetTurtle[1].xTurtle=(rand()%7+rand()%7)*70;

    score+=1;
    vetTurtle[1].yTurtle=0;
   }

   if(tempMsec-vetTurtle[2].tempo>=20)
   {
       vetTurtle[2].yTurtle+=9.5*nivel;
       vetTurtle[2].angulo+=7;
       vetTurtle[2].tempo=tempMsec;
   }
   if(vetTurtle[2].yTurtle>560 && colisao!=1)
   {
       seed=time(0);
       srand(seed);
       vetTurtle[2].xTurtle=(rand()%5+rand()%8+rand()%2)*70;


       score+=1;
       vetTurtle[2].yTurtle=0;
   }

   /// * ======== MOVIMENTO PERSONAGEM ======== * ///

    ///*xPersonagem -- *iPersonagem -- *colisao -- *tempMsec -- *tempo_mov -- *tempo_animacao
    mov_Personagem(&xPersonagem, &iPersonagem, &colisao, &tempMsec, &tempo_mov, &tempo_animacao);

   /// * ======== COLISOES ========== * ///

   func_Colisao(xPersonagem,vetTurtle[0].xTurtle,vetTurtle[0].yTurtle,&colisao);
   func_Colisao(xPersonagem,vetTurtle[1].xTurtle,vetTurtle[1].yTurtle,&colisao);
   func_Colisao(xPersonagem,vetTurtle[2].xTurtle,vetTurtle[2].yTurtle,&colisao);

   /// * ========== NIVEIS - DIFICULDADES =========== * ///
   func_Nivel(score,&nivel,&printnv);

   if(vetTurtle[0].angulo>255) vetTurtle[0].angulo=0;
   if(vetTurtle[1].angulo>255) vetTurtle[1].angulo=0;
   if(vetTurtle[2].angulo>255) vetTurtle[2].angulo=0;


   /// * ======== DESENHOS ========= * ///

   clear(buffer);
   draw_sprite(buffer,fundo,0,0);

    if(colisao==0)
   draw_sprite_ex(buffer,per[iPersonagem],xPersonagem,500,0,0);

   else
   {
     if(score!=0)finalScore=score;
     draw_sprite(buffer,gameover,0,0);
     textprintf_ex(buffer,fontscore,300,320,makecol(181,230,29), -1, "FINAL SCORE: %d", finalScore);
     nivel=1;
     score=0;
     printnv=1;
   }

   /// ========== Desenho das Turtles ======== ///
   rotate_sprite(buffer,turtle,vetTurtle[0].xTurtle,vetTurtle[0].yTurtle,ftofix(vetTurtle[0].angulo));
   rotate_sprite(buffer,turtle,vetTurtle[1].xTurtle,vetTurtle[1].yTurtle,ftofix(vetTurtle[1].angulo));
   rotate_sprite(buffer,turtle,vetTurtle[2].xTurtle,vetTurtle[2].yTurtle,ftofix(vetTurtle[2].angulo));

   if(vetTurtle[0].yTurtle>=500 && colisao!=1) draw_sprite(buffer,fail,vetTurtle[0].xTurtle,540);
   if(vetTurtle[1].yTurtle>=500 && colisao!=1) draw_sprite(buffer,fail,vetTurtle[1].xTurtle,540);
   if(vetTurtle[2].yTurtle>=500 && colisao!=1) draw_sprite(buffer,fail,vetTurtle[2].xTurtle,540);



   if(colisao!=1)
   {
     textprintf_ex(buffer,fontscore,10,10,makecol(181,230,29), -1, "SCORE: %d", score);
     textprintf_ex(buffer,fontscore,700,10,makecol(181,230,29), -1, "NIVEL: %d",printnv);
   }

   draw_sprite(screen, buffer,0,0);

 }

 /// DESTROY
 destroy_font(fontscore);
 destroy_bitmap(buffer);
 destroy_bitmap(turtle);
 destroy_bitmap(personagem);
 destroy_bitmap(fundo);

 allegro_exit();
 return 0;
}
END_OF_MAIN();

 /// ========== FUNCAO DE COLISAO ========= ///
 void func_Colisao(int xPersonagem, float xTurtle, float yTurtle, int *colisao)
 {
    if(xPersonagem>=xTurtle && xPersonagem<=xTurtle+70 && yTurtle>=500 && yTurtle<=560) *colisao=1;
    if(xPersonagem+100>=xTurtle && xPersonagem+100<=xTurtle+70 && yTurtle>=500 && yTurtle<=560) *colisao=1;
    if(xPersonagem+50>=xTurtle && xPersonagem+50<=xTurtle+70 && yTurtle>=500 && yTurtle<=560) *colisao=1;
 }


 /// ========= FUNCAO AUMENTO DE NIVEL ======== ///
 void func_Nivel(int score,float *nivel,int *printnv)
 {
   if(score>=10)  {*nivel=1.4; *printnv=2;}
   if(score>=20) {*nivel=1.6; *printnv=3;}
   if(score>=40) {*nivel=1.8; *printnv=4;}
   if(score>=60) {*nivel=2.0; *printnv=5;}
   if(score>=120) {*nivel=2.2; *printnv=6;}
   if(score>=140) {*nivel=2.4; *printnv=7;}
   if(score>=200) {*nivel=2.6; *printnv=8;}
   if(score>=300) {*nivel=2.8; *printnv=9;}
   if(score>=400) {*nivel=3.0; *printnv=10;}
 }


 /// ======= FUNCAO MOVIMENTO DE PERSONAGEM ======= ///

 void mov_Personagem(int *xPersonagem, int *iPersonagem, int *colisao, int *tempMsec, int *tempo_mov, int *tempo_animacao)
   {

      // ------------ MOVIMENTO RIGHT -------------- ///
      if(key[KEY_RIGHT] && *xPersonagem<770 && *colisao==0)
     {
      if(*tempMsec-*tempo_mov>=15)
      {
        *xPersonagem+=7;
        *tempo_mov=*tempMsec;
      }

      if(*tempMsec-*tempo_animacao>=100)
      {
        if(*iPersonagem==0) *iPersonagem=1;

        else *iPersonagem=0;

        *tempo_animacao=*tempMsec;
      }
     }

     // --------------- MOVIMENTO LEFT ----------- //
     if(key[KEY_LEFT] && *xPersonagem>0 && *colisao==0)
     {
      if(*tempMsec-*tempo_mov>=15)
      {
      *xPersonagem-=7;
      *tempo_mov=*tempMsec;
      }

      if(*tempMsec-*tempo_animacao>=100)
      {
       if(*iPersonagem==2) *iPersonagem=3;

       else *iPersonagem=2;

       *tempo_animacao=*tempMsec;
       }
     }
   }
