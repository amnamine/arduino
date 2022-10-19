
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
lcd.init();
lcd.backlight();

}

void loop() {
lcd.setCursor(2,0);
lcd.print("Projet");
lcd.setCursor(1,1);
lcd.print("Niveau EAU"); 

} 
