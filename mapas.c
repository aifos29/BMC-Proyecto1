#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mapas.h"


GtkWidget *windowInitial;
GtkWidget *g_frame_manualEntry;
GtkWidget *g_frame_fileEntry;
GtkWidget *g_filechooser_btn;
GtkWidget *g_entry_totalGen;
GtkWidget *g_entry_fileName;
GtkWidget *dialog;
GtkWidget *dialog2;

GtkWidget *zoom;

GtkWidget *windowTableData;
GtkWidget ***tableData;
GtkWidget *g_tableData;
GtkWidget *g_scrolledwindow_initialTableData;
FILE      *file_tableData;

GtkWidget *darea;

int openZoom = 0;
int totalGen;
bool debug = false;
bool error = false;
int sizeArray;


int scala = 4;

GtkWidget *window;
GtkWidget *grid;
GtkWidget *swindow;

GtkWidget *viewport;

int main() {
  if(debug){
    //createCromosmomeMaps();
  }

    GtkBuilder      *builder;



    gtk_init(NULL, NULL);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "cruces.glade", NULL);

    windowInitial = GTK_WIDGET(gtk_builder_get_object(builder, "window_initial"));
    gtk_builder_connect_signals(builder, NULL);


	  windowTableData = GTK_WIDGET(gtk_builder_get_object(builder, "window_tableData"));
    gtk_builder_connect_signals(builder, NULL);

     g_scrolledwindow_initialTableData = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_initialTableData"));

    g_frame_fileEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_fileEntry"));
    gtk_widget_hide(g_frame_fileEntry);
    g_frame_manualEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_manualEntry"));
    gtk_widget_hide(g_frame_manualEntry);

    g_filechooser_btn = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_btn"));

    g_entry_totalGen = GTK_WIDGET(gtk_builder_get_object(builder, "entry_total"));

    g_entry_fileName = GTK_WIDGET(gtk_builder_get_object(builder, "entry_fileName"));

    dialog = GTK_WIDGET(gtk_builder_get_object(builder,"errorMessage"));

    dialog2 = GTK_WIDGET(gtk_builder_get_object(builder,"errorMessage2"));

    zoom = GTK_WIDGET(gtk_builder_get_object(builder,"zoom"));

    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.cvs");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(g_filechooser_btn),filter);



    gtk_widget_show(windowInitial);
    gtk_main();

 	  gtk_widget_show(dialog);
    return 0;

}



static void do_drawing(cairo_t *, GtkWidget *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr)
{
  do_drawing(cr, widget);

  return FALSE;
}

static void do_drawing(cairo_t *cr, GtkWidget *widget)
{

    int y_size = 500;
  int x_size = 0;
  int final_size = 0;
  int y = 60;
  //printf("Chains To Print: %i\n", chainsUsed);
  for (int x = 0; x < chainsUsed; x++){
    if(activeChains[x] == 0){
      continue;
    }
    int initial = 0;
    int final = scala*5;

    //printf("Relations In Chain: %i\n", relationsInChain[x]);
    for (int i = 0; i < relationsInChain[x]; i++){
      //printf("Current relation: %i\n", i);
       int porcentaje = final + chains[x][i].value * (100*scala);

       if (chains[x][i].value >= 1){
         //Para la distancia
         initial = initial + 400;
         //Refedine el valor para segir el mismo proceso
         chains[x][i].value = chains[x][i].value - 1;
       }
       //Dibuja el gen inicial
       cairo_move_to (cr, initial,y);
       cairo_line_to (cr, final, y);
       cairo_set_source_rgb (cr, 0.2, 0.2, 6);
       cairo_set_line_width (cr, 10*scala);
       cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
       cairo_stroke (cr);
       cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
       cairo_move_to(cr, initial + 5, y);
       cairo_show_text(cr,chains[x][i].initialGene);
       cairo_set_font_size(cr,scala*2);
       //Dibuja la probabilidad que hay entre cada uno
       cairo_move_to (cr,final,y);
       cairo_line_to(cr,porcentaje,y);
       cairo_set_source_rgb (cr, 0.5, 0.5, 1);
       cairo_set_line_width (cr, 10*scala);
       cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
       cairo_stroke (cr);
       //Escribe en la imagen el texto que corresponde.
       char charray[5];
       if (chains[x][i].value != 0){
       sprintf(charray, "%1.2f", chains[x][i].value);
       cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
       cairo_move_to(cr,final,y);
       cairo_show_text(cr, charray);
       cairo_set_font_size(cr,scala*2);
       initial = porcentaje;
       final = porcentaje + (scala*5);
    }
    //printf("END FOR\n");

    }
    if (final > final_size){
      final_size = final;
    }
    y= y + (10*scala)+10;
  }
  gtk_widget_set_size_request( darea,final_size, chainsUsed*y);

if (!openZoom){
  openZoom = 1;
  gtk_widget_show(zoom);
}

}



void openDrawing(){


  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  grid = gtk_grid_new();
  swindow = gtk_scrolled_window_new (NULL,NULL);
  viewport = gtk_viewport_new (NULL,NULL);
  darea = gtk_drawing_area_new();

  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  gtk_widget_set_hexpand(GTK_WIDGET(swindow), TRUE);
  gtk_widget_set_vexpand(GTK_WIDGET(swindow), TRUE);
  gtk_container_add (GTK_CONTAINER(viewport), darea);
  gtk_container_add (GTK_CONTAINER(swindow), viewport);
  //gtk_grid_attach (GTK_GRID(grid), swindow, 0, 1, 1, 2);
  gtk_container_add (GTK_CONTAINER(window),swindow);

  g_signal_connect(G_OBJECT(darea), "draw",
      G_CALLBACK(on_draw_event), NULL);
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_title(GTK_WINDOW(window), "Mapas Cromosomicos");


  gtk_widget_show_all(window);

}

void less(){
  if (scala>0){
  gtk_widget_hide(window);
  scala = scala -1;
  openDrawing();
}
}

void more(){
  gtk_widget_hide(window);

  scala = scala+1;
  openDrawing();

}

void on_window_initial_knapsack_destroy() {
	gtk_main_quit();
}

void on_window_tableData_destroy() {
  //free(tableData);
	gtk_main_quit();
}

void on_window_finalTable_destroy() {
  //free(tableFinalData);
  gtk_main_quit();
}

void destroy_dialog(){
	gtk_widget_hide(dialog);
}

void destroy_dialog2(){
	gtk_widget_hide(dialog2);
}

void on_btn_manualEntry_clicked() {
  gtk_widget_hide(g_frame_fileEntry);
  gtk_widget_show(g_frame_manualEntry);
}

void on_btn_fileEntry_clicked() {
  gtk_widget_hide(g_frame_manualEntry);
  gtk_widget_show(g_frame_fileEntry);
}

void changeRow(GtkWidget * widget, int x){
  gtk_entry_set_text (GTK_ENTRY(tableData[0][x]),gtk_entry_get_text(GTK_ENTRY(tableData[x][0])));
}

void changeColumn(GtkWidget * widget, int x){
  gtk_entry_set_text (GTK_ENTRY(tableData[x][0]),gtk_entry_get_text(GTK_ENTRY(tableData[0][x])));
}

void createTableData(){
  int tableSize = totalGen +1;
  tableData = calloc(tableSize,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTableData), g_tableData);

  for(int j = 0; j < tableSize; j++) {
    tableData[j] = calloc(tableSize,sizeof(GtkWidget*));
  }

  for(int row =0; row < tableSize; row++)
  {
    for(int column=0; column < tableSize; column++)
    {
      tableData[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]),10);
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);

      if (column == 0 && row!=0){
      	char name[10];
      	sprintf(name, "GEN%d", row);
      	 gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),name);
         g_signal_connect(G_OBJECT(GTK_ENTRY(tableData[row][column])), "changed", G_CALLBACK(changeRow), row);

      }
        if (column != 0 && row==0){
      	char name[10];
      	sprintf(name, "GEN%d", column);
      	 gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),name);
         g_signal_connect(G_OBJECT(GTK_ENTRY(tableData[row][column])), "changed", G_CALLBACK(changeColumn),column);

      }
        if (row>column && column != 0){
             gtk_widget_set_sensitive(tableData[row][column],FALSE);
        }
        if (row==column){
            gtk_widget_set_sensitive(tableData[0][0],FALSE);
            gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),"0.0");
        }
        if (row<column && row!=0 && column !=0){
          gtk_entry_set_max_length (GTK_ENTRY(tableData[row][column]),4);
         }
  }
}
  gtk_widget_set_sensitive(tableData[0][0],FALSE);
  gtk_widget_set_name(tableData[0][0],"rowHeader");
  gtk_widget_show_all(windowTableData);

}

void on_btn_getEntries_clicked() {

	int entry_total = atoi(gtk_entry_get_text (GTK_ENTRY(g_entry_totalGen)));
	if (entry_total==0){
		gtk_widget_show(dialog);
	}

	else{
		printf("Entrada Manual\n");
		totalGen = entry_total;
		createTableData();
		gtk_widget_hide(windowInitial);
		gtk_widget_show_now(windowTableData);
	}
}

void createTableDataFile(char Data[totalGen][totalGen][5],char header[totalGen][30]){
  int tableSize = totalGen +1;
  tableData = calloc(tableSize,sizeof(GtkWidget**));

  g_tableData = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (g_scrolledwindow_initialTableData), g_tableData);

  for(int j = 0; j < tableSize; j++) {
    tableData[j] = calloc(tableSize,sizeof(GtkWidget*));
  }

  for(int row =0; row < tableSize; row++)
  {
    for(int column=0; column < tableSize; column++)
    {
      tableData[row][column] = gtk_entry_new();
      gtk_entry_set_width_chars(GTK_ENTRY(tableData[row][column]),10);
      gtk_grid_attach (GTK_GRID (g_tableData),tableData[row][column] , column, row, 1, 1);

      if (column == 0 && row!=0){
      	 gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),header[row-1]);
         g_signal_connect(G_OBJECT(GTK_ENTRY(tableData[row][column])), "changed", G_CALLBACK(changeRow), row);

      }
        if (column != 0 && row==0){
      	 gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),header[column-1]);
         g_signal_connect(G_OBJECT(GTK_ENTRY(tableData[row][column])), "changed", G_CALLBACK(changeColumn), column);

      }

      if (row != 0 && column != 0){
      	gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),Data[row-1][column-1]);
      }
      if (row>column){
             gtk_widget_set_sensitive(tableData[row][column],FALSE);
       }
       if (row<column && row!=0 && column !=0){
          gtk_entry_set_max_length (GTK_ENTRY(tableData[row][column]),4);
        }


    }
  }
  gtk_widget_set_sensitive(tableData[0][0],FALSE);
  gtk_widget_set_name(tableData[0][0],"rowHeader");
  gtk_widget_show_all(windowTableData);
}


void on_btn_getFile_clicked() {
  //header = malloc(totalObjects * sizeof(char*));
  totalGen = countObjectsFiles (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(g_filechooser_btn)));
  printf("Tamaño%d\n",totalGen );
  char Data[totalGen][totalGen][5];
  char header[totalGen][30];
  readFile(Data,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)),header);
  printf("Entrada Archivo\n");
  createTableDataFile(Data,header);
  gtk_widget_hide(windowInitial);
}

void createArray(relation information[sizeArray]){
  int position = 0;
  for (int i=1;i<totalGen+1;i++){
    for (int j=1;j<totalGen+1;j++)
      if (i<j){
        relation add;
        /*Falta agregar el nombre
        strcpy(add.initialGene,gtk_entry_get_text(GTK_ENTRY(tableData[i][0])));
        strcpy(add.finalGene,gtk_entry_get_text(GTK_ENTRY(tableData[0][j])));
        */
        if (strcmp(gtk_entry_get_text(GTK_ENTRY(tableData[i][j])),"-")!=0){
            double x = strtod(gtk_entry_get_text(GTK_ENTRY(tableData[i][j])), NULL);
            printf("%f\n",x);
            if (x>0 && x<1){
              copyString2(add.initialGene, gtk_entry_get_text(GTK_ENTRY(tableData[i][0])));
              copyString2(add.finalGene, gtk_entry_get_text(GTK_ENTRY(tableData[0][j])));
              add.value = x;
            }
            else{
              error = true;
              break;
            }
        }
        else{
          add.value = -1;
        }
        copyString(information[position].initialGene, add.initialGene);
        copyString(information[position].finalGene, add.finalGene);
        information[position].value = add.value;
        position ++;

      }
  }
}

void createFile(char *fileName) {
/*Declaración de Array*/
  sizeArray = (totalGen * totalGen-1)/2;
  relation information[sizeArray];
  createArray(information);
  printf("%d\n",error);
  if (!error){
    file_tableData = fopen(fileName,"w+");
  for(int row =0; row < totalGen+1; row++)
  {
    for(int column=0; column < totalGen+1; column++)
    {
    	if (column !=0){
    		fprintf(file_tableData,"%s;",(gtk_entry_get_text(GTK_ENTRY(tableData[row][column]))));
    	}


  }
    fprintf(file_tableData,"\n");
  }
  fclose(file_tableData);

  createCromosmomeMaps(information, sizeArray);
  printf("\n\nRESULT\nChains Used: %i\n", chainsUsed);
  bool valid = false;
  for(int i = 0; i < chainsUsed; i++){
    if(activeChains[i] == 1){
      valid = true;
    }
  }
  if(!valid){
    gtk_widget_show(dialog2);
  } else {
    openDrawing(); 
  }
}
  else{

    printf("Datos ingresados no válidos\n");
    error = false;
    gtk_widget_show(dialog2);

  }
}

void processData(){

}

void on_btn_getTableData_clicked() {
	int lenName = strlen(gtk_entry_get_text (GTK_ENTRY(g_entry_fileName))) + 21;

	char fileName[lenName];
	strcpy(fileName,"examples/");
	strcat(fileName, gtk_entry_get_text (GTK_ENTRY(g_entry_fileName)));
  strcat(fileName, ".cvs");
  printf("%s\n",fileName);
  createFile(fileName);

}
