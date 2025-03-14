#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/******************
Name:Omri Halfon
ID:324209402
Assignment:Ex 5
*******************/
// Struct Definitions
typedef struct {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

typedef struct {
    char *name;
    Song **songs;
    int songsNum;
} Playlist;
typedef struct String{
    char c;
    struct String *next;
}String;
enum mainMenuCases{
     caseExit=4,
     caseWatchPlaylist=1,
     caseAddPlaylist=2,
     caseRemovePlaylist=3,
 };enum mainMenuCases;
enum playlistMenu {
    ShowPlaylist=1,
    AddSong=2,
    RemoveSong=3,
    SortPlaylist=4,
    PlayAll=5,
    ExitPlaylistMenu=6
};enum playlistMenu;
 enum sortMenu {
    SortYear=1,
    SortStreamsAscending=2,
    SortStreamsDescending=3,
};enum sortMenu;
enum Exit {
    Failure=1,
    Success=0
};enum Exit;
enum getString {
    oneChar=1,
    twoChars=2
};enum getString;
enum Bool {
    False=0,
    True=1
};enum Bool;
// Function Prototypes

// Reads a string from user input
char *getString();

//Displays the main menu options
void printMainMenu();

// Adds a new playlist
void addPlaylist(Playlist ***playlists, int *playlistCount);

//remove playlist from playlists
void removePlaylist(Playlist ***playlists, int *playlistCount, int choice);

//display playlists
void watchPlaylists(Playlist **playlists, int playlistCount);

//playlist menu function
void playlistMenu(Playlist *playlist);

//display songs and play by user choice
void showPlaylist(Playlist *playlist);
//add song to playlist
void addSong(Playlist *playlist);
//display all songs and asked the user which song to remove
int deleteSongPrinting(Playlist *playlist);
//delete song from playlist
void deleteSong(Playlist *playlist, int choice);
//helper function to sort by streams ascending order
void SortByStreamsAsc(Song **songs, int n);
//helper function to sort by streams descending order
void SortByStreamsDesc(Song **songs, int n);
//helper function to sort by alphabetically
void SortByTitle(Song **songs, int n);
//helper function to sort by year
void SortByYear(Song **songs, int n);
//display the sort songs menu
int sortMenu();
//function to sort the songs in playlist
void sort(Playlist *playlist);
//play all songs function
void playAllSongs(Playlist *playlist);
//free memory of single playlist
void freePlaylist(Playlist ***playlists, int *playlistCount,int choice);
void clearInputBuffer();
int main() {
    //// Dynamic array of playlists
    Playlist **playlists = NULL;
    //number of playlists
    int playlistCount = 0;
    int choice = 0;
    do {
        printMainMenu();
        if (scanf("%d", &choice)) {
            clearInputBuffer();
            switch (choice) {
                case caseExit: {
                    break;
                }
                case caseWatchPlaylist: {
                    watchPlaylists(playlists, playlistCount);
                    break;
                }
                case caseAddPlaylist: {
                    addPlaylist(&playlists, &playlistCount);
                    break;
                }
                case caseRemovePlaylist: {
                    printf("Choose a playlist:\n");
                    for (int i = 0; i < playlistCount; i++) {
                        int temp=i;
                        temp++;
                        //display all playlists names ask the user which playlist to remove
                        printf("\t%d. %s\n", temp, playlists[i]->name);
                    }
                    int backToMainMenu = playlistCount;
                    backToMainMenu++;
                    printf("\t%d. Back to main menu\n", backToMainMenu);
                    int playlistToRemove;
                    //get the user's choice
                    scanf("%d", &playlistToRemove);
                    //if the user chose back to main menu , return to main menu
                    if (playlistToRemove == backToMainMenu) {
                        break;
                    }
                    playlistToRemove--;
                    removePlaylist(&playlists, &playlistCount,playlistToRemove);
                    break;
                }
                default: {
                    break;
                }
            }
        } else {
            int c;
            // Clear invalid input
            while ((c = getchar()) != '\n' && c != EOF);
            printf("“Invalid option”\n");
        }
    } while (choice != caseExit);
    //free every single playlist
    int lastPlaylist = playlistCount;
    lastPlaylist--;
    for (int i = lastPlaylist; i >= 0; i--) {
        freePlaylist(&playlists, &playlistCount, i);
    }

    if (playlists) {
        free(playlists);
        playlists = NULL;
    }
    printf("Goodbye!\n");

    return Success;
}
void printMainMenu() {
    printf("Please Choose:\n\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}
char *getString() {
    char *string = NULL;
    int size = 0;
    char c;
    //while the user's input different from '\n'
    while ((c = getchar()) != '\n' && c != EOF) {
        //initialize new string every single char and left place for '\0' at the end of the sring
        int newSize = size + twoChars;
        char *temp = realloc(string, newSize * sizeof(char));
        //if reallocation failed free everything
        if (!temp) {
            free(string);
            printf("memory allocation error");
            exit(Failure);
        }
        //get the new char into string
        string = temp;
        string[size] = c;
        //increase the length of the string by 1 char
        size++;
    }
    //if string is not null get the last char of the string by null char
    if (string) {
        string[size] = '\0';
    }
    //if the input is empty get one null char into the string
    else {
        string = malloc(oneChar);
        if (!string) {
            printf("Memory allocation error\n");
            exit(Failure);
        }
        string[0] = '\0';
    }
    //if the user get new line at the end of the string replace it by null char
    if (string[size - oneChar] == '\r') {
        string[size - oneChar] = '\0';
    }
    //return the input
    return string;
}
void addPlaylist(Playlist ***playlists, int *playlistCount) {
    printf("Enter playlist's name:\n");
    char *name = getString();
    Playlist *newPlaylist = malloc(sizeof(Playlist));
    //if malloc failed free everything
    if (!newPlaylist) {
        printf("Memory allocation error\n");
        free(name);
        exit(Failure);
    }
    //get the name of the playlist , initialize the songs of the playlist to null and the number of songs to 0
    newPlaylist->name = name;
    newPlaylist->songs = NULL;
    newPlaylist->songsNum = 0;
    //increase the playlists by one more playlist
    int newSizeOfPlaylists = *playlistCount;
    newSizeOfPlaylists++;
    Playlist **newPlaylists = realloc(*playlists, sizeof(Playlist *) * newSizeOfPlaylists);
    //if reallocation failed free everything
    if (!newPlaylists) {
        free(name);
        free(newPlaylist);
        printf("Memory allocation error\n");
        exit(Failure);
    }
    //change the pointers of playlists to the new playlists , increase the number of playlist by one
    *playlists = newPlaylists;
    (*playlists)[*playlistCount] = newPlaylist;
    (*playlistCount)++;
}
void watchPlaylists(Playlist **playlists, int playlistCount) {
    while (1) {
        printf("Choose a playlist:\n");
        for (int i = 0; i < playlistCount; i++) {
            int temp=i;
            temp++;
            printf("\t%d. %s\n", temp, playlists[i]->name);
        }
        int backToMainMenu = playlistCount;
        backToMainMenu++;
        printf("\t%d. Back to main menu\n", backToMainMenu);
        int choice;
        if (scanf("%d", &choice) == 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            if (choice > 0 && choice <= backToMainMenu) {
                //if the user's choice is back to main menu
                if (choice == backToMainMenu) {
                    return; // exit to main menu
                }
                int lastPlaylist = playlistCount;
                lastPlaylist--;
                printf("playlist %s:\n",playlists[lastPlaylist]->name);
                playlistMenu(playlists[lastPlaylist]); // Manage the selected playlist
            } else {
                printf("Invalid option.\n");
            }
        } else {
            //in case of invalid option
            printf("Invalid option.\n");
            clearInputBuffer();
        }
    }
}
void playlistMenu(Playlist *playlist) {
    int choice;
    do {
        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
        if(scanf("%d", &choice)) {
            clearInputBuffer();
            switch (choice) {
                case ShowPlaylist: {
                    showPlaylist(playlist);
                    break;
                }
                case AddSong: {
                    addSong(playlist);
                    break;
                }
                case RemoveSong: {
                    int songToDelete=deleteSongPrinting(playlist);
                    if(songToDelete==False) {
                        break;
                    }
                    deleteSong(playlist,songToDelete);
                    break;
                }
                case SortPlaylist: {
                    sort(playlist);
                    break;
                }
                case PlayAll: {
                    playAllSongs(playlist);
                    break;
                }
                default: {
                    break;
                }
            }
        }else {
            clearInputBuffer();
        }
    }while (choice != ExitPlaylistMenu);
}
void showPlaylist(Playlist *playlist) {
    //display all songs of the playlist
    if(playlist->songsNum > 0) {
        for (int i = 0; i < playlist->songsNum; i++) {
            int nextIndex = i;
            nextIndex++;
            printf("%d.\tTitle: %s\n\tArtist: %s\n\tReleased: %d\n\tStreams: %d\n",nextIndex,
                playlist->songs[i]->title,playlist->songs[i]->artist,playlist->songs[i]->year,
                playlist->songs[i]->streams);
        }
    }
    int choice;
    //loop through the user's choice to quit else play selected single song of the user
    while(1){
        //ask the user to choose a song to play , update the streams of the selected song
        printf("choose a song to play, or 0 to quit:\n");
        scanf("%d", &choice);
        int choiceIndex=choice;
        choiceIndex--;
        //in case the user asked to play invalid song
        if(choice > (playlist->songsNum)) {
            continue;
        }
        //if the user's choice is valid or not to quit play the chosen song
        else if (choice) {
            printf("Now playing %s:\n$ %s $\n",playlist->songs[choiceIndex]->title,playlist->songs[choiceIndex]->lyrics);
            playlist->songs[choiceIndex]->streams++;
        }
        //when user asked to quit break the loop
        if(!choice) {
            break;
        }
    }
}
void addSong(Playlist *playlist) {
    printf("Enter song's details\n");
    printf("Title:\n");
    //get the title input
    char *title = getString();
    //get the artist input from user
    printf("Artist:\n");
    char *artist = getString();
    //if artist is null return back

    //get the year from the user
    printf("Year of release:\n");
    int year;
    scanf("%d", &year);
    clearInputBuffer();
    //get the song's lyrics from user
    printf("Lyrics:\n");
    //get the new line char
    char *lyrics = getString();
    //create a new song pointer size of one song
    Song *newSong = malloc(sizeof(Song));
    //if malloc failed free everything
    if (!newSong) {
        printf("Memory allocation error\n");
        free(title);
        free(artist);
        free(lyrics);
        exit(Failure);
    }
    //get all the song's data into the new song pointer
    newSong->title = title;
    newSong->artist = artist;
    newSong->year = year;
    newSong->lyrics = lyrics;
    newSong->streams=0;
    //increase the song's array of the specific playlist by one more song
    playlist->songs=realloc(playlist->songs, sizeof(Song *) * (playlist->songsNum + 1));
    //in case reallocation failed free every single byte
    if (!playlist->songs) {
        printf("Memory allocation error\n");
        free(title);
        free(artist);
        free(lyrics);
        free(playlist->songs);
        exit(Failure);
    }
    //get the new song into the songs array of the chosen playlist, increase the number of song in playlist by 1
    playlist->songs[playlist->songsNum] = newSong;
    playlist->songsNum++;
}
int sortMenu() {
    int choice=0;
    printf("choose:\n1. sort by year\n2. sort by streams - ascending order\n3. sort by streams - descending order\n"
           "4. sort alphabetically\n");
    scanf("%d", &choice);
    return choice;
}
void SortByYear(Song **songs, int numOfSongs) {
    int lastSong = numOfSongs;
    lastSong--;
    for (int i = 0; i < lastSong; i++) {
        for (int j = 0; j < lastSong-i; j++) {
            int nextIndex=j;
            nextIndex++;
            if (songs[j]->year > songs[nextIndex]->year) {
                Song *temp = songs[j];
                songs[j] = songs[nextIndex];
                songs[nextIndex] = temp;
            }
        }
    }
}

void SortByStreamsAsc(Song **songs, int numOfSongs) {
    int lastSong = numOfSongs;
    lastSong--;
    for (int i = 0; i < lastSong; i++) {
        for (int j = 0; j < lastSong-i; j++) {
            int nextIndex=j;
            nextIndex++;
            if (songs[j]->streams > songs[nextIndex]->streams) {
                Song *temp = songs[j];
                songs[j] = songs[nextIndex];
                songs[nextIndex] = temp;
            }
        }
    }
}

void SortByStreamsDesc(Song **songs, int numOfSongs) {
    int lastSong = numOfSongs;
    lastSong--;
    for (int i = 0; i < lastSong; i++) {
        for (int j = 0; j < lastSong-i ; j++) {
            int nextIndex=j;
            nextIndex++;
            if (songs[j]->streams < songs[nextIndex]->streams) {
                Song *temp = songs[j];
                songs[j] = songs[nextIndex];
                songs[nextIndex] = temp;
            }
        }
    }
}
void SortByTitle(Song **songs, int numOfSongs) {
    int lastSong = numOfSongs;
    lastSong--;
    for (int i = 0; i < lastSong; i++) {
        for (int j = 0; j < lastSong-i; j++) {
            int nextIndex=j;
            nextIndex++;
            if (strcmp(songs[j]->title, songs[nextIndex]->title) > 0) {
                Song *temp = songs[j];
                songs[j] = songs[nextIndex];
                songs[nextIndex] = temp;
            }
        }
    }
}

void sort(Playlist *playlist) {
    int choice=sortMenu();
    switch (choice) {
        case SortYear: {
            //sort by years
            SortByYear(playlist->songs, playlist->songsNum);
            break;
        }
        case SortStreamsAscending: {
            //sort by streams ascending order
            SortByStreamsAsc(playlist->songs, playlist->songsNum);
            break;
        }
        case SortStreamsDescending: {
            //sort by streams descending order
            SortByStreamsDesc(playlist->songs, playlist->songsNum);
            break;
        }
        default: {
            //default sort alphabetically
            SortByTitle(playlist->songs, playlist->songsNum);
        }
    }
    printf("sorted\n");
}
void playAllSongs(Playlist *playlist) {
    //display all songs title and lyrics increase the streams of the songs
    for (int i = 0; i < playlist->songsNum; i++) {
        printf("Now playing %s:\n$ %s $\n",playlist->songs[i]->title,playlist->songs[i]->lyrics);
        playlist->songs[i]->streams++;
    }
}
int deleteSongPrinting(Playlist *playlist) {
    //display all songs ask the user which song to delete
    if (playlist->songsNum > 0) {
        for (int i = 0; i < playlist->songsNum; i++) {
            int nextIndex=i;
            nextIndex++;
            printf("%d.\tTitle: %s\n\tArtist: %s\n\tReleased: %d\n\tStreams: %d\n",
                   nextIndex, playlist->songs[i]->title,
                   playlist->songs[i]->artist, playlist->songs[i]->year, playlist->songs[i]->streams);
        }
    }
    //in case no songs in playlist
    else {
        printf("No songs in the playlist.\n");
        return False;
    }
    int choice;
    printf("choose a song to delete, or 0 to quit:\n");
    //return the index of the chosen song to delete
    scanf("%d", &choice);
    return choice;
}
void deleteSong(Playlist *playlist, int choice) {
    choice--;
    if (choice < 0 || choice >= playlist->songsNum) {
        return; // Exit if invalid choice
    }
    // Free memory for the selected song
    free(playlist->songs[choice]->title);
    free(playlist->songs[choice]->artist);
    free(playlist->songs[choice]->lyrics);
    free(playlist->songs[choice]);

    // Shift songs to fill the gap
    int lastSong = playlist->songsNum;
    lastSong--;
    for (int i = choice; i < lastSong; i++) {
        int nextIndex=i;
        nextIndex++;
        playlist->songs[i] = playlist->songs[nextIndex];
    }

    // Reduce the size of the array
    playlist->songsNum--;

    // Reallocate memory to reduce the array size
    Song **newSongs = realloc(playlist->songs, sizeof(Song *) * playlist->songsNum);
    if (!newSongs && playlist->songsNum > 0) {
        printf("Memory allocation error\n");
        exit(Failure);
    }

    playlist->songs = newSongs;

    printf("Song deleted successfully.\n");
}
void removePlaylist(Playlist ***playlists, int *playlistCount, int choice) {
    if (choice < 0 || choice >= *playlistCount) {
        return; // Invalid choice
    }

    // Free playlist name
    free((*playlists)[choice]->name);

    // Free all songs in the playlist
    for (int i = 0; i < (*playlists)[choice]->songsNum; i++) {
        free((*playlists)[choice]->songs[i]->title);
        free((*playlists)[choice]->songs[i]->artist);
        free((*playlists)[choice]->songs[i]->lyrics);
        free((*playlists)[choice]->songs[i]);
    }

    // Free the songs array
    free((*playlists)[choice]->songs);

    // Free the playlist itself
    free((*playlists)[choice]);

    // Shift remaining playlists to fill the gap
    int lastIndex=*playlistCount;
    lastIndex--;
    for (int i = choice; i < lastIndex; i++) {
        int temp=i;
        temp++;
        (*playlists)[i] = (*playlists)[temp];
    }

    // Reduce the size of the array
    (*playlistCount)--;
    Playlist **newPlaylists = realloc(*playlists, sizeof(Playlist *) * (*playlistCount));
    if (!newPlaylists && *playlistCount > 0) {
        printf("Memory allocation error\n");
        exit(Failure);
    }
    *playlists = newPlaylists;
    printf("Playlist deleted.\n");
}
void freePlaylist(Playlist ***playlists, int *playlistCount, int choice) {
    if (choice < 0 || choice >= *playlistCount) {
        printf("Invalid playlist index\n");
        return;
    }

    // Free playlist name
    free((*playlists)[choice]->name);

    // Free all songs in the playlist
    for (int i = 0; i < (*playlists)[choice]->songsNum; i++) {
        free((*playlists)[choice]->songs[i]->title);
        free((*playlists)[choice]->songs[i]->artist);
        free((*playlists)[choice]->songs[i]->lyrics);
        free((*playlists)[choice]->songs[i]);
    }

    // Free the songs array
    free((*playlists)[choice]->songs);

    // Free the playlist itself
    free((*playlists)[choice]);

    // Shift remaining playlists to fill the gap
    int lastPlaylist=*playlistCount;
    lastPlaylist--;
    for (int i = choice; i < lastPlaylist; i++) {
        int nextIndex=i;
        nextIndex++;
        (*playlists)[i] = (*playlists)[nextIndex];
    }

    // Reduce the size of the array
    (*playlistCount)--;
    if (*playlistCount > 0) {
        Playlist **newPlaylists = realloc(*playlists, sizeof(Playlist *) * (*playlistCount));
        if (!newPlaylists) {
            printf("Memory allocation error during resizing playlists\n");
            exit(Failure);
        }
        *playlists = newPlaylists;
    } else {
        free(*playlists);
        *playlists = NULL;
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
