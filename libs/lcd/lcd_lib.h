/*=============================================================================

LCD_LIB_de	Funktionen fuer die LCD-Anzeige des MiniMEXLE
=========================================================

Dateiname: lcd_lib_de.h

Bibliothek fuer das LCD-Display des MiniMexle Boards

Autor: 		Steffen Freihofer (2006)
Adaption: 	Thomas Pospiech (2007, Timing-Probleme geloest)
			G. Gruhler, D. Chilachava (2009, Init Ports und Dokumentation)

Version: 	1.2	vom 25.02.2009

=============================================================================*/


// Deklarationen ==============================================================

// Festlegung der Quarzfrequenz

#ifndef F_CPU				// optional definieren
#define F_CPU 18432000UL 	// MiniMEXLE mit 18,432 MHz Quarz
#endif


// Include von Header-Dateien

#include <avr/io.h>			// I/O Konfiguration (intern weitere Dateien)
#include <avr/interrupt.h>	// globale Interrupts definieren (sei() und cli())
#include <stdint.h>			// Definition von Typen (int, char, ...)
#include <stdbool.h>		// Definition von 1-Bit-Variablentypen (bool)
#include <util/delay.h>		// Definition von Delays (Wartezeiten)


// Konstanten

#define CLEAR_DISPLAY	1	// Instruction Code fuer LCD: Loeschen des Displays


// Port-Bits

#define E 7					// Enable-Signal zum Display: Port D, Bit 7
#define RS 4				// Register-Select zum Display: Port D, Bit 4


// Makros

#define P_DATA 	  PORTC		// Port C ist Datenport zum LCD
#define P_STEUER  PORTD		// Port D ist Steuerport zum LCD


// Funktionsprototypen

void lcd_enable (void);
void lcd_write (unsigned char byte);
void lcd_init (void);
void lcd_putc (unsigned char zeichen);
void lcd_putstr (char *string);
void lcd_gotoxy (unsigned char line, unsigned char pos);
void lcd_clearDisplay(void);
void lcd_clearline (unsigned char line);
void lcd_displayMessage(char *string, unsigned char, unsigned char);

