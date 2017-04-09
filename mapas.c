#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


GtkWidget *windowInitial;
GtkWidget *g_frame_manualEntry;
GtkWidget *g_frame_fileEntry;
GtkWidget *g_filechooser_btn;
GtkWidget *g_entry_totalGen;
GtkWidget *g_entry_fileName;
GtkWidget *dialog;
GtkWidget *windowTableData;
GtkWidget ***tableData;
GtkWidget *g_tableData;
GtkWidget *g_scrolledwindow_initialTableData;
int totalGen;

int main() {

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


    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.cvs");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(g_filechooser_btn),filter);



    gtk_widget_show(windowInitial);                
    gtk_main();
	
 	gtk_widget_show(dialog);
    return 0;

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

void on_btn_manualEntry_clicked() {
  gtk_widget_hide(g_frame_fileEntry);
  gtk_widget_show(g_frame_manualEntry);
}

void on_btn_fileEntry_clicked() {
  gtk_widget_hide(g_frame_manualEntry);
  gtk_widget_show(g_frame_fileEntry);
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
      }
        if (column != 0 && row==0){
      	char name[10];
      	sprintf(name, "GEN%d", column);
      	 gtk_entry_set_text (GTK_ENTRY(tableData[row][column]),name);
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




void on_btn_getFile_clicked() {
  printf("Entrada Archivo\n");

}

void on_btn_getTableData_clicked() {
	int lenName = strlen(gtk_entry_get_text (GTK_ENTRY(g_entry_fileName))) + 21;
  
	char fileName[lenName]; 
	strcpy(fileName,"examples/Mochila/");
	strcat(fileName, gtk_entry_get_text (GTK_ENTRY(g_entry_fileName)));
    strcat(fileName, ".cvs");
  	printf("%s\n",fileName);
}