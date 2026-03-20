//      Box und Deckel 26.2.2926 khf
//              TM-Clock

//*********   Auswahl   

 //   1:Box  2:Deckel 

    part = "2";

//*********************************************


//**********   Parmeter   *********************
x = 82;   //Breite
y = 102;  //Länge
z = 50;   //Höhe incl Deckel
d1 = 4;   //Wand-Dicke mind. 3mm
b1 = 0;   //Durchmesser Bohrung mind. 3mm
//*********************************************


a=3;
d= d1<a ? 0 : d1;

c=3;
b= b1<c ? 0 : b1;


print_part();

 
module print_part() 
 {
	if (part == "1") { 
        
        difference() {
		Box();
                                    
           
//Durchbruch Spannungsversorgung
    rotate([90,0,90] ){ 
    translate(v = [-30, -10, 30])
    cylinder(h = 15, r1 = 2.5, r2 = 2.5);
}           

//Durchbruch USB
            //translate(v = [-15, -50, -13.7])
            //cube(size = [25,11,6], center=true);
            
//Durchbruch Siebesegment-Anzeige
            rotate([90, 90, 0]) {         
            translate(v = [-1, -38, 0])
            cube(size = [20.5,21,50.5], center=true);
            }
            

         } 
  	} 
        
    else if (part == "2") {
        difference() {		
        Deckel();
/*        
 //Senklöcher                   
translate([36.5, 46.5, 3])
cylinder(h = 7, r1 = 1, r2 = 5, center = true);

translate([-36.5, 46.5, 3])
cylinder(h = 7, r1 = 1, r2 = 5, center = true);

translate([36.5, -46.5, 3])
cylinder(h = 7, r1 = 1, r2 = 5, center = true);

translate([-36.5, -46.5, 3])
cylinder(h = 7, r1 = 1, r2 = 5, center = true);
            */
        
        }        

    } 
}


include <box/box_lib.scad>;

/*
translate([-8, 50, 6.5])
cube(size = [2,2,9.3], center = true/false);

translate([-5, 50, 11])
cube(size = [40,4,5], center = true/false);

translate([-35, 50, 11])
cube(size = [23,4,5], center = true/false);



translate([22.5, -54, -21])
cube(size = [17.3,4,5], center = true/false);

translate([22.5, -54, -30])
cube(size = [17.3,11,12.5], center = true/false);


            
            
            
translate([-40, -54, -30])
cube(size = [38,11,8], center = true/false); //Abstand links


translate([-22.5, -54, -30])
cube(size = [17.3,11,8], center = true/false); //Abstand unten


*/

difference() {
translate([-38, -48, 1])
   cube(size = [76,96,4], center = true/false);            

translate([-36, -47, 2])
cylinder(h = 5, r1 = 4, r2 = 4);

translate([36, -47, 2])
cylinder(h = 5, r1 = 4, r2 = 4);    


translate([-36, 46, 2])
cylinder(h = 5, r1 = 4, r2 = 4);

translate([36, 46, 2])
cylinder(h = 5, r1 = 4, r2 = 4);
}








