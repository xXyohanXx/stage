#include "Jeu_text.h"
#include "winTxt.h"

int main(){
    
    Jeu_text j;
    j.commencer();
    //termClear();
	
	j.txtBoucle();
	termDysplayInput(true);
    termClear(); // a laisser absolument sinon problème de mémoire 
    
    
	return 0;

    
}