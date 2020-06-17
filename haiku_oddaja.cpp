#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cassert>

#include <sstream>
#include <algorithm>
#include <iterator>

#include <memory>

#include <time.h>

#include <typeinfo>

using namespace std;



enum Vrsta{ samostalnik, prislov, glagol, pridevnik, veznik };
enum Spol{ m,z,s };



bool vseb(string big, string small){
	return big.find(small) != std::string::npos;
}

string koncnica_brez(string kon){
	return kon.substr(1,kon.size()-1);
}


void split(const std::string& str, vector<string>& cont, char delim = ' ')
{
	cont.clear();
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

string prvi(string s){ 
	if (vseb(s, " ")){
		vector<string> ve;
		split(s,ve, ' ');
		return ve[0].substr(0,ve[0].size()-1);
	}else{
		return s;
	}
}

string sptostr(Spol e)
{
  switch (e)
  {
  case m: return "m";
  case z: return "z";
  case s: return "s";
  }
  return "error";
}

Spol strtosp(string e)
{
	Spol sp;
	if (e == "m"){
		sp = m;
	}else if(e == "ž"){
		sp = z;
	}else if(e == "s"){
		sp = s;
	}else{
		cout << e << " # ";
	}
	return sp;
}

int st_samoglas(string in){

	int st = 0;
	vector<char> v = {'a','e','i','o','u'};

	for (char e:in){
		for (char f : v){
			if (e == f){
				++st;
				break;
			}
		}
	}

	if(st == 0){
		st = in.size()/2;
	}

	return st;
}






//#############STRUCT#########################################################33

struct Beseda{
	Vrsta vrsta;
	string beseda;
	string rezerva;
	int stevilo;

	int status;

	virtual ostream& print(ostream& os) const {
			return os << "";
		}

	virtual vector<int> get_() = 0;

	virtual void set(int ind, vector<Beseda*>& sez) = 0;

	

	virtual string get_bes() = 0;

};

ostream& operator<<(ostream& os, const Beseda& o) {
	return o.print(os);
}

struct Samostalnik : public Beseda{
	string beseda;
	string rk;
	Spol spol;
	Vrsta vrsta = samostalnik;
	string kvalifikator;

	int status = 1;
	string rezerva;

	int stevilo;
	int sklon;

	

	ostream& print(ostream& os) const override {
		Beseda::print(os);
		return os << beseda + " " ;//+ "	 (sam) "  << sptostr(spol) << "    " << kvalifikator << "   "<< rezerva << "    st: " << stevilo << " sk: " << sklon <<  endl;
	}

	vector<int> get_(){
		return {status, stevilo, spol, sklon};
	}

	void set(int ind, vector<Beseda*>& sez) override {
		// predlog pri orodniku in mestniku če povratni == 1


		string prim = "";

		//sklon
		bool ali = 1;
		for (int j = ind-1; j>0; --j){
			if (vseb(typeid(*sez[j]).name(), "Glagol")){
				ali = 0;
				if(sez[j]->get_()[0]){
					sklon = 3;
					sez[j]->status = 0;
					break;
				}
				sklon = 0;
				
				//...
				 
				break;
			}
		}
		if (ali){
			sklon = 0;
			
		}
		
		


		//stevilo 

		ali = 1;
		for (size_t j = ind+1; j<sez.size(); ++j){
			if (vseb(typeid(*sez[j]).name(), "Glagol")){
				ali = 0;
				stevilo = (sez[j]->get_())[1];
				break;
			}
		}
		if (ali){
			stevilo = 0;
			
		}

		
		


		vector<vector<vector<string>>> vzor = {
			/*0*/{{"","a","u","","u","om"},{"a","ov","oma","a","ih","oma"},{"i","ov","om","e","ih","i"}},
			/*1*/{{"","a","u","","u","em"},{"a","ev","ema","a","ih","ema"},{"i","ev","em","e","ih","i"}},
								/*če živo z a-jem*/
			/*2*/{{"a", "e", "i", "o", "i", "o"}, {"i", "", "oma", "i", "ah", "ama"}, {"e", "","am","e","ah","ami"}},
			/*3*/{{"a", "e", "i", "o", "i", "o"}, {"i", "","ama", "i", "ah","ama"}, {"e", "", "am", "e", "ah", "ami"}},
			/*4*/{{"", "i","i","","i","jo"}, {"i","i","ma","ti","ih","ma"}, {"i","i","im", "i","ih","mi"}},
			/*5*/{{"o", "a", "u", "o","u","om"}, {"i","","oma","i","ih","oma"}, {"a","","om","a","ih","u"}},
			/*6*/{{"e", "a", "u", "e","u","em"}, {"i","","ema","i","ih","ema"}, {"a","","em","a","ih","u"}},
			/*7*/{{"ev", "ve", "vi", "vo", "vi", "vijo"}, {"vi", "ev","vama", "vi", "vah","vama"}, {"ve", "ev", "vam", "ve", "vah", "vami"}}
		};
		

		int sk;

		string crka = rk.substr(rk.size()-1,1);
		string crka2 = beseda.substr(beseda.size()-1,1);

		if (crka =="a"&&spol==m){

			
			sk = 0;

			///////////// sk, sklon, stev //////////////7
			string koncnica = vzor[sk][stevilo][sklon];
			string podaljsava = "";
			if(koncnica.size()!=0){
				podaljsava = rk.substr(0,rk.size()-1);
			}

			
			if (podaljsava.size()>1){
				size_t fi = beseda.find(rk.substr(0,1));
				prim = beseda.substr(0,fi) + rk.substr(0,rk.size()-1) + vzor[sk][stevilo][sklon];
			}else{
				prim = beseda +  podaljsava + koncnica;
			}


			
			
			
		}
		else if(crka =="e"&&spol==m){
			sk=2;
		}
		else if(crka =="e"&&spol==z){
			if (beseda.substr(beseda.size()-2,2).compare("ev") == 0){
				sk = 7;
				prim = beseda.substr(0,beseda.size()-2) + vzor[sk][stevilo][sklon];
			}
			else{
				sk = 3;
				prim = beseda.substr(0,beseda.size()-1) + vzor[sk][stevilo][sklon];
			}
			
		}
		else if(crka =="i"&&spol==z){
			sk=4;
			prim = beseda + vzor[sk][stevilo][sklon];
		}
		else if(crka2 =="o"&&spol==s){
			sk=5;
			prim = beseda.substr(0,beseda.size()-1) + vzor[sk][stevilo][sklon];
			
		}
		else if(crka2 =="e"&&spol==s){
			sk=6;
			prim = beseda.substr(0,beseda.size()-1) + vzor[sk][stevilo][sklon];
			
		}
		else{
			cout << "neeeeeeeeeeeee sam" << endl;
			
		}

		

		rezerva = beseda;
		beseda = prim;


	}

	string get_bes(){
		return beseda;
	}


	Samostalnik(string beseda, string rk, Spol spol, string kvalifikator) : beseda(beseda), rk(rk), spol(spol), kvalifikator(kvalifikator){};



};

struct Pridevnik : public Beseda{

	
	string beseda;
	Vrsta vrsta = pridevnik;
	string k;

	int status = 1;
	string rezerva;
	string osnova;

	int stevilo;
	int sklon;
	Spol spol;
	
	
	

	ostream& print(ostream& os) const override {
		Beseda::print(os);
		return os << beseda + " " ;
	}

	vector<int> get_(){
		return {status, stevilo};
	}

	void set(int ind, vector<Beseda*>& sez) override {
		//stevilo, spol, sklon

		for (size_t j = ind+1; j<sez.size(); ++j){
			if (vseb(typeid(*sez[j]).name(), "Samostalnik")){
				auto ve = sez[j]->get_();
				stevilo = ve[1];
				spol = static_cast<Spol>(ve[2]);
				sklon = ve[3];
				break;
			}
		}

		///////////////////////////////////////////////////////////

		vector<vector<vector<vector<string>>>> vzor = {
			/*kon i*/{
					/*m*/{
							{"","ega","emu","i"/*neživo*/,"em","im"},
							{"a","ih","ima","a","ih","ima"},
							{"i","ih","im","e","ih","imi"}
						},
					/*z*/{
							{"a","e","i","o","i","o"},
							{"i","ih","ima","i","ih","ima"},
							{"e","ih","im","e","ih","imi"}

						},
					/*s*/{
							{"o","ega","emu","o","em","im"},
							{"i","ih","ima","i","ih","ima"},
							{"a","ih","im","a","ih","imi"}
						}
					}
		};

		rezerva = beseda;
		string prim;

		if (beseda.compare(beseda.size()-1,1,"i") == 0){
			if (sklon==0 && stevilo==0 && spol==0){
				prim = beseda.substr(0,beseda.size()-1) + "i";
			}
			else{
				prim = beseda.substr(0,beseda.size()-1) + vzor[0][spol][stevilo][sklon];
			}
			

		}
		else if (beseda.compare(beseda.size()-2,2,"en") == 0){
			if (sklon==0 && stevilo==0 && spol==0){
				prim = beseda;
			}
			else{
				prim = beseda.substr(0,beseda.size()-2) + "n" + vzor[0][spol][stevilo][sklon];
			}
		}
		else{
			prim = beseda + vzor[0][spol][stevilo][sklon];
		}



		beseda = prim;
		
	}

	string get_bes(){
		return beseda;
	}

	Pridevnik(string beseda, string k) : beseda(beseda), k(k){};


};

struct Glagol : public Beseda {
	Vrsta vrsta = glagol;
	string beseda;
	string k;

	bool status = 1;
	string rezerva;
	bool povratni = 0;

	int oseba;
	int stevilo;

	

	ostream& print(ostream& os) const override {
		Beseda::print(os);
		return os << beseda + " " ;
	}

	vector<int> get_(){
		return {status, stevilo, povratni};
	}

	void set(int ind, vector<Beseda*>& sez) override {

		rezerva = beseda;

		
		string osnova;
		string _s;
		string prim;

		
		if(vseb(beseda, " se") || vseb(beseda, " si")){
			size_t fi = beseda.find(' ');
			_s = beseda.substr(fi+1, beseda.size()-fi);
			beseda = _s + " " + beseda.substr(0, fi);
			povratni = 1;
		}

		k = k.substr(0, k.size()-1); //-m

		if (vseb(k,"-")==0){
				prim = k;
		}
		//ov
		else if (vseb(beseda, "ov") || vseb(beseda, "ev")){
			size_t ind = beseda.size()-5;
			osnova = beseda.substr(0,ind);
			prim = osnova + koncnica_brez(k);
		}
		//am em im
		else if(vseb("aeiou", beseda.substr(beseda.size()-3,1)) && k.size()==2){
				osnova = beseda.substr(0,beseda.size()-3);
				prim = osnova + koncnica_brez(k);					
		}

		else if(vseb(beseda.substr(0, beseda.size()-3),k.substr(1,1))){
			osnova = beseda.substr(0, beseda.find(k.substr(1,1)));
			prim = osnova + koncnica_brez(k);
		}
		else{
			cout << "napaka, neveljaven glagol, d(k) = " << k.size() << "k: " << k << endl;
			osnova = beseda.substr(0,beseda.size()-3);
			prim = osnova + koncnica_brez(k);
		}
		
		
		

		bool ali = 1;
		for (int j = ind-1; j>=0; --j){
			if (vseb(typeid(*sez[j]).name(), "Samostalnik")){
				oseba = 2;
				ali = 0;
				break;
			}
		}
		if(ali == 1){
			srand(time(NULL));
			oseba = rand() % 2;
		}
		srand(time(NULL));
		stevilo = rand() % 3;

		vector<vector<string>> konk = {{"m", "š", ""}, {"va", "ta", "ta"}, {"mo", "te", "jo"}};
		beseda = prim + konk[stevilo][oseba];
	}

	string get_bes(){
		return beseda;
	}

	Glagol(string beseda, string k) : beseda(beseda), k(k){};


};

struct Prislov : public Beseda {

};

struct Veznik : public Beseda {

};





Spol sp(string s){
	char c = s[0];
	if (int(c)==int('ž')){
		c='z';
	}
	Spol r = (Spol)c;
	return r;
}







int main(){


	
	string s;
	vector<Pridevnik> pridevniki;
	vector<Samostalnik> samostalniki;
	vector<Glagol> glagoli;
	vector<Prislov> prislovi;
	vector<Veznik> vezniki;

	vector<string> v;
	vector<string> t;

	bool pogoj = 0;
	int stevec = 0;
	int stevec2 = 100;

	while(pogoj == 0 && stevec < 10){
		++stevec;
		stevec2 -= 2*stevec;

		while(getline(cin, s)){
			split(s,v,';');
			if (vseb(s, "samostalnik")){

				string sp, kvalifikator;
				// mn neskl ed
				if(vseb(v[3],",")){
					split(v[3], t, ',');
					sp = t[0];
					kvalifikator = t[1];
				}else{
					sp = v[3];
					kvalifikator = "";
				}

				if (prvi(v[1]).size()>0){
					auto rkm = koncnica_brez(prvi(v[1]));
					if(rkm.find(',') != std::string::npos){ 
						rkm.erase(rkm.find(','),1);
					}
					
					Samostalnik sam (v[0], rkm, strtosp(sp), kvalifikator);
					samostalniki.push_back(sam);
				}
				
				
			}
			else if (vseb(s, "pridevnik")){
				Pridevnik prid (v[0], prvi(v[1]));
				pridevniki.push_back(prid);
			}
			else if (vseb(s, "glagol")){
				Glagol gl (v[0], prvi(v[1]));
				glagoli.push_back(gl);

			}
			

			
		}


		vector<Vrsta> oblika;
		oblika.push_back(pridevnik);
		oblika.push_back(samostalnik);
		oblika.push_back(glagol);
		oblika.push_back(pridevnik);
		oblika.push_back(samostalnik);

		vector<Beseda*> haiku;

		Beseda* ele;
		Samostalnik* a;
		Pridevnik* b;
		Glagol* c;
		int n = 9;
		for (Vrsta vr : oblika){

			if (vr == samostalnik){
				srand (time(NULL )+ stevec*stevec2+n);
				a = &samostalniki[rand() % samostalniki.size()];
				ele = dynamic_cast<Beseda*>(a);
			}
			else if (vr == pridevnik){
				srand (time(NULL )+ stevec*stevec2 +1+n);
				b = &pridevniki[rand() % pridevniki.size()];
				ele = dynamic_cast<Beseda*>(b);
				
			}
			else if (vr == glagol){
				srand (time(NULL )+ stevec*stevec2 +2+n);
				c = &glagoli[rand() % glagoli.size()];
				ele = dynamic_cast<Beseda*>(c);
			}
			else{
				cout << "bla" << endl;
			}
			haiku.push_back(ele);

			++n;

		}

		for (size_t i = 0; i<haiku.size(); ++i){
			if (vseb(typeid(*haiku[i]).name(), "Glagol")){
				haiku[i]->set(i, haiku);
			}
		}

		for (size_t i = 0; i<haiku.size(); ++i){
			if (vseb(typeid(*haiku[i]).name(), "Samostalnik")){
				haiku[i]->set(i, haiku);
			}
		}

		for (size_t i = 0; i<haiku.size(); ++i){
			if (vseb(typeid(*haiku[i]).name(), "Pridevnik")){
				haiku[i]->set(i, haiku);
			}
		}


		int st_zlogov = 0;
		for (size_t q = 0; q<haiku.size(); q++){
			st_zlogov += st_samoglas(haiku[q]->get_bes());
			
		}
		

		if (st_zlogov>=15 && st_zlogov<=20){

			int umesno_st_zlogov = 0;
			

			for (size_t q = 0; q<haiku.size(); q++){
				cout << *haiku[q];
				umesno_st_zlogov += st_samoglas(haiku[q]->get_bes());
				if (umesno_st_zlogov > 4){
					cout << endl;
					umesno_st_zlogov = 0;
				}
				
			}
			cout << endl << endl;

		}


		
	}

	

	


	return 0;
}