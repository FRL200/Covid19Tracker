#include <iostream>
#include  <stack>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include<queue>
#include <fstream>

using namespace std;

struct Location{

    string name;
    double vaccinationRate;
    double cases;
    string type;


};

map<string,vector<pair<string,Location>>> WorldGraph;

vector<Location> getAdjacent(string vertex)
{

    vector<Location> result;
    for(int i=0; i<WorldGraph[vertex].size();i++) {
        result.push_back(WorldGraph[vertex][i].second);
    }
    return result;

}
vector<Location> bfs(string src) {
    map<string,bool> visited;
    queue<string> queue;
    vector<Location> bfsVector;
    visited[src] =true;
    queue.push(src);

    while(!queue.empty()){

        string i = queue.front();
        map<string,vector<Location>> adjacents;
        adjacents[i]=getAdjacent(i);
        bfsVector.push_back(WorldGraph[i][0].second);
        queue.pop();
        for (Location neighbors: adjacents[i]) {
            if (!visited[neighbors.name]){
                queue.push(neighbors.name);
                visited[neighbors.name]= true;
            }

        }

    }
    return bfsVector;


}
bool sortByVaccination(Location location1, Location location2)
{
    return location1.vaccinationRate < location2.vaccinationRate;
}
vector<Location> ListTop10Vaccinated(){
    vector <Location> result;
    vector <Location> FinalResult;

    for (int i = 0; i < bfs("World").size(); i++) {

        result.push_back(bfs("World")[i]);

    }
    sort(result.begin(),result.end(),sortByVaccination);
    for (int i = 0; i < 10; i++) {

                FinalResult.push_back(result[result.size()-i-1]);

    }
    return FinalResult;



}

vector<Location> ListTop10LeastVaccinated(){
    vector <Location> result;
    vector <Location> FinalResult;

    for (int i = 0; i < bfs("World").size(); i++) {

        result.push_back(bfs("World")[i]);

    }
    sort(result.begin(),result.end(),sortByVaccination);
    for (int i = 0; i < 10; i++) {

        FinalResult.push_back(result[i]);

    }
    return FinalResult;





}
vector<Location> dfs(string src) {
    map<string,bool> visited;
    stack<string> stack;
    vector<Location> dfsVector;
    visited[src] =true;
    stack.push(src);


    while(!stack.empty()){

        string i = stack.top();
        map<string,vector<Location>> adjacents;
        adjacents[i]=getAdjacent(i);
        dfsVector.push_back(WorldGraph[i][0].second);
        stack.pop();
        for (Location neighbors: adjacents[i]) {
            if (!visited[neighbors.name]){
                stack.push(neighbors.name);
                visited[neighbors.name]= true;
            }

        }

    }
    return dfsVector;


}
void insertEdge(string location, Location to)
{

    WorldGraph[location].push_back(make_pair(to.name,to));
    if(WorldGraph.find(location)==WorldGraph.end()){
        WorldGraph[location]={};
    }


}

Location Search(string location){
    Location result;
    if (WorldGraph[location].empty()){
        result.name=" ";
        result.cases= 0;
        result.type= " ";
        result.vaccinationRate=0;
        return result;
    }
    else {
        for (int i = 0; i < WorldGraph[location].size(); i++) {
            if(WorldGraph[location][i].first==location){
                return WorldGraph[location][i].second;
            }
        }
    }


}
void Compare(string locationOne,string locationTwo){


    Location one=Search(locationOne);

    cout<<"First Location"<<endl;
    cout<<"Location Name: "<<one.name<<endl;
    cout<<"Vaccination Percentage: "<<one.vaccinationRate<<endl;
    cout<<"Covid Cases Percentage: "<<one.cases<<endl;
    cout<<endl;

    cout<<"Second Location"<<endl;
    Location two=Search(locationTwo);
    cout<<"Location Name: "<<two.name<<endl;
    cout<<"Vaccination Percentage: "<<two.vaccinationRate<<endl;
    cout<<"Covid Cases Percentage: "<<two.cases<<endl;

}

void ListCountries(string continents){

    for (int i = 0; i < WorldGraph[continents].size(); i++) {
        if(WorldGraph[continents][i].second.type=="Country"){

           cout<<"Location Name: "<<Search(WorldGraph[continents][i].first).name<<endl;
            cout<<"Vaccination Percentage: "<< Search(WorldGraph[continents][i].first).vaccinationRate<<endl;
            cout<< "Covid Cases Percentage: "<<Search(WorldGraph[continents][i].first).cases<<endl;
        }
        cout<<endl;
    }



}
void listContinents(){


    for (int i = 0; i < getAdjacent("World").size(); i++) {
        cout<<"Location Name: "<<getAdjacent("World")[i].name<<endl;
        cout<<"Vaccination Percentage: "<<getAdjacent("World")[i].vaccinationRate<<endl;
        cout<<"Covid Cases Percentage: "<<getAdjacent("World")[i].cases<<endl;
        cout<<endl;

    }

}

void createLocation(string name, int vaccinationRate, int cases, string type){
    Location newLocation;
    newLocation.name=name;
    newLocation.vaccinationRate=vaccinationRate;
    newLocation.cases=cases;
    newLocation.type=type;
    WorldGraph[newLocation.name].push_back(make_pair(newLocation.name,newLocation));


}

void ParseInfo(){
    ifstream inFS;
    inFS.open("ParseDATA2(Done).csv");
    string line;
    while (!inFS.eof()) {
        vector<string> data;
        string str;
        getline(inFS, line);
        for (long unsigned int i = 0; i < line.size(); i++) {
            if (line[i] == ',') {
                data.push_back(str);
                str = "";
            }
            else{
                str += line[i];
            }
        }
        data.push_back(str);
        for (int i = 0; i < data.size(); i++) {
            if(data[i] == "" || data[i] == "\r"){
                data[i]= "0";
            }
        }
        string type;
        if(data[0].empty()){
            type= "Continent";
        }
        else{
            type= "Country";
        }

        createLocation(data[1], stod(data[7])*100,stod(data[6])*100, type);
        if (type=="Continent"){
            insertEdge("World",WorldGraph[data[1]][0].second);
        }
        else {
            insertEdge(data[0], WorldGraph[data[1]][0].second);
        }
    }
    inFS.close();
}


void randomNames(){
    string str ="abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 150000; i++) {
        string name;
        for (int j = 0; j < rand() % 30 + 1; j++) {
            name += str[rand() % 26];
        }
        int num = rand() % (65 + 30);
        int num2 = rand() % (50+ 12);

        createLocation(name,num, num2,"Country");
        insertEdge("Universe",WorldGraph[name][0].second);
    }


}

int main() {
    createLocation("Universe",0,0,"Universe");
    //createLocation("World",1,1,"World");



   ParseInfo();
    insertEdge("Universe",WorldGraph["World"][0].second);
   randomNames();
    insertEdge("World",WorldGraph["Africa"][0].second);
    insertEdge("World",WorldGraph["South America"][0].second);
    insertEdge("World",WorldGraph["North America"][0].second);
    insertEdge("World",WorldGraph["Oceania"][0].second);
    insertEdge("World",WorldGraph["Europe"][0].second);
    insertEdge("World",WorldGraph["Asia"][0].second);

    //menu

    bool running = true;

    while (running)
    {
        cout << " ---------- Menu Options ----------" << endl;
        cout << "0. Exit" << endl;
        cout << "1. List top 10 most vaccinated countries" << endl;
        cout << "2. List top 10 least vaccinated countries" << endl;
        cout << "3. Add Connection" << endl; //insert edge
        cout << "4. List countries from specific continent" << endl;
        cout << "5. List info of all continents" << endl;
        cout << "6. Search specific location" << endl;
        cout << "7. Add a Location"<<endl;
        cout << "8. Compare two locations"<<endl;
        cout << "9. Breadth First Traversal"<<endl;
        cout << "10. Get Connected Locations"<<endl;
        cout  << endl;
        cout << "Which menu option would you like to choose?" << endl;

        int menuOption;

        cin >> menuOption;

        //case 3/7
        string startLoc = "";
        string destinationName = "";
        int vRate = 0;
        int numOfCases = 0;
        string typeOfLoc;

        //case 4
        string continentName = "";

        //case 6
        string searchName = "";

        switch (menuOption)
        {
            case 0:
                cout<<"Thank you for using the Covid 19 Tracker";
                running = false;
                break;
            case 1:
                for (int i = 0; i < ListTop10Vaccinated().size(); i++) {
                    cout<<"Location Name: "<<ListTop10Vaccinated()[i].name<<endl;
                    cout<<"Vaccination Percentage: "<<ListTop10Vaccinated()[i].vaccinationRate<<endl;
                    cout<<"Covid Cases Percentage: "<<ListTop10Vaccinated()[i].cases<<endl;
                    cout<< endl;
                }
                break;
            case 2:

                for (int i = 0; i < ListTop10LeastVaccinated().size(); i++) {
                    cout<<"Location Name: "<<ListTop10LeastVaccinated()[i].name<<endl;
                    cout<<"Vaccination Percentage: "<<ListTop10LeastVaccinated()[i].vaccinationRate<<endl;
                    cout<<"Covid Cases Percentage: "<<ListTop10LeastVaccinated()[i].cases<<endl;
                    cout<< endl;

                }
                break;
            case 3:
                cout << "Add starting location: ";
                cin >> startLoc;
                cout << "" << endl;

                cout << "Add destination's name: ";
                cin >> destinationName;
                cout << "" << endl;

                insertEdge(startLoc,WorldGraph[destinationName][0].second);
                cout<<"Successfully created connection"<<endl;

                break;

            case 4:
                cout << "Which continent's countries would you like to see? ";
                cin >> continentName;
                cout << "" << endl;

                ListCountries(continentName);

                break;
            case 5:
                listContinents();
                break;

            case 6:
                cout << "Which location would you like to search? ";
                cin>>searchName;
                cout <<endl;
                cout<<"Location Name: "<<Search(WorldGraph[searchName][0].first).name<<endl;
                cout<<"Vaccination Percentage: "<<Search(WorldGraph[searchName][0].first).vaccinationRate<<endl;
                cout<<"Covid Cases Percentage: "<<Search(WorldGraph[searchName][0].first).cases<<endl;

                break;
            case 7:
                cout << "Location name: ";
                cin >> startLoc;
                cout << "" << endl;

                cout << "Add Location's vaccination rate: ";
                cin >> vRate;
                cout << "" << endl;

                cout << "Add Location's percentage of cases: ";
                cin >> numOfCases;
                cout << "" << endl;

                cout << "What is the type of location? (ex: City, Country, or Continent) ";
                cin >> typeOfLoc;
                cout << "" << endl;

                createLocation(startLoc,vRate,numOfCases,typeOfLoc);
                cout<<"Successfully created location "<<startLoc<<endl;
                break;

            case 8:
                cout << "First Location name: ";
                cin>> startLoc;
                cout<< endl;

                cout<< "Second Location name: ";
                cin>>destinationName;
                cout<<endl;

                Compare(startLoc,destinationName);
                break;
            case 9:
                cout<< "Starting location name: ";
                cin>> startLoc;
                cout<<endl;
                for (int i = 0; i < bfs(startLoc).size(); ++i) {
                    cout<<"Location Name: "<<bfs(startLoc)[i].name<<endl;
                    cout<<"Vaccination Percentage: "<<bfs(startLoc)[i].vaccinationRate<<endl;
                    cout<<"Covid Cases Percentage: "<<bfs(startLoc)[i].cases<<endl;
                    cout<<endl;

                }
                break;
            case 10:
                cout<< "Location name: ";
                cin>> startLoc;
                cout<< endl;
                for (int i = 0; i < getAdjacent(startLoc).size(); i++) {
                    cout<<"Location Name: "<<getAdjacent(startLoc)[i].name<<endl;
                    cout<<"Vaccination Percentage: "<<getAdjacent(startLoc)[i].vaccinationRate<<endl;
                    cout<<"Covid Cases Percentage: "<<getAdjacent(startLoc)[i].cases<<endl;
                    cout<<endl;

                }
                break;
        }

    }

    return 0;
}