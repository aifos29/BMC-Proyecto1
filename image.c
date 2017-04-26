#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include "mapas.h"

static void do_drawing(cairo_t *, GtkWidget *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{  
  do_drawing(cr, widget);  

  return FALSE;
}

static void do_drawing(cairo_t *cr, GtkWidget *widget)
{
  GtkWidget *win = gtk_widget_get_toplevel(widget);
  
  int width, height;
  gtk_window_get_size(GTK_WINDOW(win), &width, &height);

  relation data[4];
  relation new;
  strcpy(new.initialGene,"A");
  strcpy(new.finalGene,"C");
  new.value = 0.80;
  data[0] = new;
    

  strcpy(new.initialGene,"C");
  strcpy(new.finalGene,"F");
  new.value = 0.60;
  data[1] = new;

  strcpy(new.initialGene,"F");
  strcpy(new.finalGene,"B");
  new.value = 0.80;
  data[2] = new;

  strcpy(new.initialGene,"B");
  strcpy(new.finalGene,"B");
  new.value = 0.00;
  data[3] = new;
  

  int initial = 0;
  int final = 15;
  for (int i = 0;i<4;i++){
  	 int porcentaje = final + data[i].value * 100;
//GEN
	 cairo_move_to (cr, initial,60);
     cairo_line_to (cr, final, 60); 
     cairo_set_source_rgb (cr, 0.2, 0.2, 6);
     cairo_set_line_width (cr, 40.0);
     cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
     cairo_stroke (cr); 	
     cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
     cairo_move_to(cr, initial +2, 60);
     cairo_show_text(cr,data[i].initialGene);

     cairo_move_to (cr,final,60);
     cairo_line_to(cr,porcentaje,60);
     cairo_set_source_rgb (cr, 0.5, 0.5, 1);
     cairo_set_line_width (cr, 40.0);
     cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
     cairo_stroke (cr); 
     char charray[5]; 
     if (data[i].value != 0){
     sprintf(charray, "%1.2f", data[i].value);
     cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
     cairo_move_to(cr,final + 10 ,40);
     cairo_show_text(cr, charray);
     initial = porcentaje;
     final = porcentaje + 15;
 	}

  }



  cairo_translate(cr, 10/2, 10/2 );
  cairo_scale(cr, 0.1, 0.2);
  cairo_fill(cr);
  cairo_stroke(cr);


 
}


int main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;
  
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(G_OBJECT(darea), "draw", 
      G_CALLBACK(on_draw_event), NULL);
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200); 
  gtk_window_set_title(GTK_WINDOW(window), "Fill & stroke");

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}