// Music Library
// February 2023
// System: VSCode on MacOS
// Author: Nuha Abougoash 

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "helper_functions.cpp"
#include <sstream>

using namespace std;

////////////////////////////////////////////// LOAD //////////////////////////////////////////////

//  Appends the file contents into the music library.
void loadCommand(string &remains, map <string, set <string>  > &artists, map <string, set <string>  > &songs, set <string> &albums){
    string file_name;
    if (!remains.empty()) { //if there is a filename
        file_name = remains; //set filename equal to remains
    }
    else if (remains.empty()) { //if there is no filename
        file_name = "musicdatabase.txt";  //set filename equal to this txt file
    }

    ifstream filein(file_name); 

    //prints error if file does not exist
    if (!filein.is_open()){
        cout << "Error: Could not open music library file - " << file_name << endl; 
    }

    // Goes through each line and sorts data accordingly
    string album;
    while(getline(filein, album)){ 
        albums.insert(album); // insert all album name lines into albums SET
        
        string line;
        getline(filein, line); 

        // conintues looping as long as album name is not repeated
        while (album != line) {
            // find the songs and insert into song MAP, where album is KEY and songs are the SET VALUES
            if (isdigit(line.at(0)) && isdigit(line.at(1))){
                songs[album].insert(line);
            }
            // find the artists and insert into artist MAP, where artist is KEY and album is SET VALUES
            else {
                artists[line].insert(album);
            }
            getline(filein, line); 
        }
    }
    filein.close();
}

////////////////////////////////////////////// STATS //////////////////////////////////////////////

// Displays the totals for the loaded music library. The total albums, number of unique artists, and number of songs
// Adding the -d option will dump the entire music library to the console.
void statsCommand(string file_name, string remains, map <string, set <string> > &artists, map <string, set <string> > &songs, set <string>& albums){
    // count number of values in song map, this will be the number of songs
    int num_songs = 0;
    for (const auto& s : songs) {
        num_songs+= s.second.size();
    }

    // print out display and size of total albums, number of unique artists, and number of songs
    cout << "Overall Music Library Stats" << endl;
    cout << "===========================" << endl;
    cout << "Total Records: " << albums.size() << endl;
    cout << "Total Unique Artists: " <<  artists.size() << endl;
    cout << "Total Songs: " <<  num_songs << endl;
    cout << endl;

    if (!remains.empty()) { ///if user inputed '-d' then display entire music library
        cout << "Your Current Music Library Includes" << endl;
        cout << "===================================" << endl;

        for (const auto& a : albums){
            // print out album name
            cout << a << endl;
            for (const auto& b  : artists ) { 
                for(const auto& b_albums : b.second) { 
                    // check if artisit is unique to album then print artisit
                    if(b_albums == a){
                        cout << " " << b.first << endl;
                    }
                }
            }
            // grabs the songs that are unique to album then print out songs
            for (const auto& c  : songs.at(a) ) { 
                cout << "   " << c << endl;
            }
        }
        cout << endl;
    }    
}

////////////////////////////////////////////// CLEAR //////////////////////////////////////////////

// Resets the music library to an empty state, using .clear() function
void clearCommand(map <string, set <string> >& artists, map <string, set <string> > &songs, set <string> &albums){
    albums.clear();
    artists.clear();
    songs.clear();
}

////////////////////////////////////////////// EXPORT //////////////////////////////////////////////

//  Exports the music library to the provided file.
void exportCommand(string remains, map <string, set <string> > &artists, map <string, set <string> > &songs, set <string>& albums){
    string export_file;
    if (!remains.empty()) { ///if there is a filename
        export_file = remains; //set filename equal to remains
    }
    else if (remains.empty()) { //if there is no filename
        export_file = "musicdatabase.txt";  //set filename equal to this txt file
    }
    
    // Write, the album, unique artist, and all songs into ofstream file
    ofstream fileout(export_file);
    for (const auto& a : albums){
            fileout << a << endl;
            for (const auto& b  : artists ) { 
                for(const auto& b_albums : b.second) { 
                    if(b_albums == a){
                        fileout << b.first << endl;
                    }
                }
            }
            for (const auto& c  : songs.at(a) ) { 
                fileout  << c << endl;
            }
            fileout << a << endl;
        }
    fileout.close();
}

////////////////////////////////////////////// SEARCH //////////////////////////////////////////////

// Return a set containing all albums with matching album
set <string> searchAlbums(string keyword, set <string> &albums){
    set <string> matchingAlbums; //empty set 
    tolower(keyword);

    // loop thru each album in albums set (a)
    for (const auto& a: albums) {
        string lower_a = a;
        tolower(lower_a);

        // if keyword found, insert album name into matchingAlbums
        if (lower_a.find(keyword) !=string::npos) {
            matchingAlbums.insert(a);
        } 
    }
    return matchingAlbums;
}

// Return a set containing all albums with matching artists
set <string> searchArtists(string keyword, map <string, set <string> > artists) {
    set <string> matchingAlbums;
    tolower(keyword);

    // loop thru artisits map 
    for (const auto& a: artists) {
        // search through KEYS of artisits for keyword
        string lower_a = a.first;
        tolower(lower_a); 
        
        // if keyword found, insert album name into matchingAlbums
        if (lower_a.find(keyword) !=string::npos) {
            matchingAlbums.insert(a.second.begin(), a.second.end());
        }
    }
    return matchingAlbums;
}

// Return a set containing all albums with matching songs
set <string> searchSongs(string keyword, map <string, set <string> > songs) {
    tolower(keyword);
    set <string> matchingAlbums;

    // loop thru songs map 
    for (const auto& s: songs) {  //
        // search through VALUES of songs for key word
        for (const auto& song : s.second){
            string lower_song = song;
            tolower(lower_song);

            // if keyword found, insert album name into matchingAlbums
            if (lower_song.find(keyword) != string::npos) { 
                matchingAlbums.insert(s.first);
            }
        }
    }
    return matchingAlbums;
}

// Split all the words at every space and insert each word into a set
void splitString(string all_words, set<string> &words) {
    stringstream ss(all_words);
    string word;
    while (ss >> word) {
        words.insert(word);
    }
}

// Search for a set of albums in the library.
void searchCommand(string remains, map <string, set <string> >& artists, map <string, set <string> > &songs, set <string> &albums) {
    set<string> words; // will hold each word user inputed, can be used to loop thru
    string type;
    string all_words;
    splitFirstWord(remains, type, all_words);  // for ex : type = artist & word = taylor swift

    // print error is the search is empty
    if (type.empty()) {
        cout << "Error: Search terms cannot be empty." << endl;
        cout << "No results found." << endl;
        cout << endl;
    }

    splitString(all_words, words); // separates each word at every space 
    
    // check the type of search user inputted (album, artist, or song), then proceed
    if (type == "album") {

        set<string> validAlbums; 
        set<string> matchingAlbums;
        
        cout << "Your search results exist in the following albums: " << endl;

        // loop thru each word user inputed and call searchAlbums function to return a set of all albums matching the word
        for (auto const& word: words) {  

            //  if the word has '-' or '+' then first remove operator, then search for all the matching albums 
            if ( word.at(0) == '-') {
                string temp;
                for(u_int i = 1; i < word.size(); i++ ) { 
                    temp.push_back(word.at(i));
                }   
                matchingAlbums = searchAlbums(temp, albums); 
            }

            else if (word.at(0) == '+') {
                string temp;
                for(u_int i = 1; i < word.size(); i++ ) { 
                    temp.push_back(word.at(i));
                }   
                matchingAlbums = searchAlbums(temp, albums);
            }

            else {
                matchingAlbums = searchAlbums(word, albums); 
            }
            
            // print all the matching albums
            for (auto const& a: matchingAlbums) {
                cout <<  a << endl;
            }
        }
        cout << endl;
    }
    
    if (type == "artist") {
        set<string> validArtists;
        set<string> matchingAlbums;

        // loop thru each word user inputed and call searchAlbums function to return a set of all albums matching the word
        for (auto const& word: words) {

            // if the word has '-' or '+' then first remove operator, then search for all the matching albums
            if ( word.at(0) == '-') {
                string temp;
                for(u_int i = 1; i < word.size(); i++ ) { 
                    temp.push_back(word.at(i));
                }  
                matchingAlbums = searchArtists(temp, artists); 
                validArtists = matchingAlbums;
            }

            else if (word.at(0) == '+') {
                string temp;
                for(u_int i = 1; i < word.size(); i++ ) { 
                    temp.push_back(word.at(i));
                }  
                matchingAlbums = searchArtists(temp, artists);
                validArtists = matchingAlbums;
            }

            else {
                matchingAlbums = searchArtists(word, artists);
                set<string> all;
                set_union(validArtists.begin(), validArtists.end(), matchingAlbums.begin(), matchingAlbums.end(), inserter(all, all.begin()));
                validArtists = all;
            }
        }
        // print all the matching albums
        cout << "Your search results exist in the following albums: " << endl;
        for (auto const& a: validArtists) {
            cout  << a << endl;
        }
        cout << endl;
    }
    
    if(type == "song") {
        set<string> validSongs;
        set<string> matchingAlbums;

        // loop thru each word user inputed 
        for (auto const& word: words) {
            // if the word has '-' or '+' then first remove operator, then search for all the matching albums
            if ( word.at(0) == '-') {
                string temp;
                for(u_int i = 1; i < word.size(); i++ ) { 
                    temp.push_back(word.at(i));
                }  
                // return a set of all albums matching the word
                matchingAlbums = searchSongs(temp, songs);
                validSongs = matchingAlbums;
            }

            else if (word.at(0) == '+') {
                string temp;
                for(u_int i = 1; i < word.size(); i++ ){ 
                    temp.push_back(word.at(i));
                }  
                matchingAlbums = searchSongs(temp, songs);
                validSongs = matchingAlbums;
            }

            else {
                matchingAlbums = searchSongs(word, songs);
                set<string> all;
                set_union(validSongs.begin(), validSongs.end(), matchingAlbums.begin(), matchingAlbums.end(), inserter(all, all.begin()));
                validSongs = all;
            }

        }
        // print all the matching albums
        cout << "Your search results exist in the following albums: " << endl;
        for (auto const& a: validSongs) {
            cout  << a << endl;
        }
        cout << endl;
    }
}

////////////////////////////////////////////// MAIN //////////////////////////////////////////////

int main() {
    string userEntry;
    string command, remains;
    string file_name;
    string export_file;
    map <string, set <string> > artists; 
    map <string, set <string> > songs;
    set <string> albums;

    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
    cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;
        getline(cin, userEntry);
        cout << endl;

        // Split the user entry into two pieces
        splitFirstWord(userEntry, command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")
        {
            clearCommand(artists, songs, albums);
        }
        else if (command == "export")
        {
            exportCommand(remains, artists, songs, albums);
        }
        else if (command == "load")
        {
            loadCommand(remains, artists, songs, albums);
        }
        else if (command == "stats")
        {
            statsCommand(file_name, remains, artists, songs, albums);
        }          
        else if (command == "search")
        {
            searchCommand(remains, artists, songs, albums);
        }

    } while(command != "exit");    

    cout << "Thank you for using the Music Library App" << endl;
    return 0;
}
