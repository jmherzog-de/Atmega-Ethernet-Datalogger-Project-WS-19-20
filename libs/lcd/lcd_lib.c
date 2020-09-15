#include "lcd_lib.h"


// LCD-Ansteuerfunktionen =====================================================

// ----------------------------------------------------------------------------
// LCD_ENABLE:		Erzeugt einen HIGH-Impuls des Enable-Signals
// ----------------------------------------------------------------------------

void lcd_enable (void)
{
	P_STEUER |=(1<<E);		// E=1   1  +-+
	P_STEUER &=~(1<<E);		// E=0   0--+ +---
}


// ----------------------------------------------------------------------------
// LCD_WRITE:	Schreibt ein Byte im 4-Bit-Modus in das LCD
// ----------------------------------------------------------------------------

void lcd_write (unsigned char byte)	// Ausgabewert 8 Bit in "byte"
{
	unsigned char temp;		// Definition lokale Variable "temp"

	temp = byte;			// Ausgabewert zwischenspeichern

	P_DATA &= 0xF0;			// Untere 4 Bit auf Datenport loeschen
	P_DATA |= (temp>>4);	// Ausgabewert 4 Bit nach rechts schieben
	//    oberes Nibble auf Daten-Port LCD schreiben

	lcd_enable();			// Erzeugung Enable-Impuls zur Datenspeicherung
	// keine Wartezeit zwischen Nibbles notwendig

	byte &= 0x0F;			// Unteres Nibble Ausgabewert extrahieren

	P_DATA &= 0xF0;			// Untere 4 Bit auf Datenport loeschen
	P_DATA |= byte;			// unteres Nibble auf Daten-Port LCD schreiben

	lcd_enable();			// Erzeugung Enable-Impuls zur Datenspeicherung
}


// ----------------------------------------------------------------------------
// LCD_INIT:	Initialisiert das LCD
// ----------------------------------------------------------------------------

void lcd_init (void)
{
	cli();						//globale Interrupts deaktivieren

	DDRC = 0x0f;				// Port C, Bit 0..3 (LCD-Daten) auf Output
	DDRD |= ((1<<E) | (1<<RS));	// Port D, Bit 4 (RS: LCD Register Select)
	//     und Bit 7 (E: LCD Enable) auf Output
	PORTC = 0x0f;				// Port C, Bit0..3 (LCD-Daten) SET

	// Steuersignale auf LOW
	P_STEUER &=~((1<<E) | (1<<RS));	// E und RS auf 0 setzen
	_delay_ms (20);

	// Display in 4-bit Modus initialisieren
	P_DATA &=0xf0;
	P_DATA |=0x03;
	lcd_enable();
	_delay_ms (10);
	lcd_enable ();
	_delay_ms (10);
	lcd_enable ();
	
	P_DATA &=1;
	_delay_ms (2);
	lcd_enable();
	_delay_us (50);
	
	// 2 Zeilen, 5 x 7 Pixel
	lcd_write (0x28);
	_delay_us (40);
	
	// Display einschalten
	lcd_write (0x0C);
	_delay_us (50);
	
	// Cursor auf "increment mode" schalten
	lcd_write (0x02);
	_delay_us (50);

	//	Display loeschen (funktioniert nur, wenn es 2x aufgerufen wird)
	lcd_write (0x01);
	_delay_ms (3);
	lcd_write (0x01);
	_delay_ms (3);

	sei();						// globale Interrrupts aktivieren
}


// ----------------------------------------------------------------------------
// LCD_PUTC:	Schreibt ein Zeichen an die entsprechende Speicherstelle
// ----------------------------------------------------------------------------

void lcd_putc (unsigned char character)	// Ausgabewert 8-Bit in "character"
{
	P_STEUER |= (1<<RS);		// Register Select auf HIGH: "Daten ausgeben"
	
	lcd_write (character);		// Zeichen ausgeben

	_delay_us (55);				// Verzoegerung 55 us (interne Verarbeitung)
}


// ----------------------------------------------------------------------------
// LCD_PUTSTR:	Schreibt einen String auf das LCD
// ----------------------------------------------------------------------------

void lcd_putstr (char *string)	// Pointer auf String in "string"
{
	while (*string)				// solange nicht Stringendezeichen 0x00
	{
		lcd_putc (*string);		// Zeichen aus String holen und ausgeben

		string ++;				// Pointer auf naechstes Zeichen setzen
	}
}


// ----------------------------------------------------------------------------
// LCD_GOTOXY:	Verschiebt den Curser an die angegebene Position
// ----------------------------------------------------------------------------

void lcd_gotoxy (unsigned char line, unsigned char pos)
{
	P_STEUER &= ~(1<<RS);		// Register Select auf LOW: "Control ausgeben"
	
	lcd_write((1<<7)+0x40*line+pos); // Adresse Cursor in DDRAM-Adresse

	_delay_us (50);				// Verzoegerung 50 us fuer Interne Verarbeitung
}


// ----------------------------------------------------------------------------
// LCD_CLEARDISPLAY:	Loescht den gesamten Display-Inhalt
// ----------------------------------------------------------------------------

void lcd_clearDisplay(void)
{
	P_STEUER &= ~(1<<RS);		// Register Select auf LOW: "Control ausgeben"
	
	lcd_write(CLEAR_DISPLAY);	// Befehlscode an Display ausgeben

	_delay_ms (2);				// Verzoegerung 2 ms fuer interne Verarbeitung

}


// ----------------------------------------------------------------------------
// LCD_CLEARLINE:	Loescht den Inhalt einer Display-Zeile
// ----------------------------------------------------------------------------

void lcd_clearline (unsigned char line)
{
	switch(line)
	{
		case 0: 	lcd_gotoxy(0,0); // Beginn erste Zeile addressieren
		lcd_putstr ("                "); // Leerzeichen ausgeben
		break;

		case 1: 	lcd_gotoxy(1,0); // Beginn zweite Zeile addressieren
		lcd_putstr ("                "); // Leerzeichen ausgeben
		break;
	}
}


// ----------------------------------------------------------------------------
// LCD_DISPLAYMESSAGE:	Anzeige einer Stringvariablen auf dem Display
// ----------------------------------------------------------------------------

void lcd_displayMessage(char* mess, unsigned char line, unsigned char pos)
{
	lcd_gotoxy(line,pos);		// Cursor positionieren
	lcd_putstr(mess);			// String ausgeben
}

