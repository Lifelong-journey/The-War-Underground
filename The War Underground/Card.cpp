#include "Card.h"
#include "Define.h"

#include <iostream>
#include <Windows.h>

using namespace std;



void Card::setCard(int ply, int tp, int l)
{
	mineral = CardMineral[tp];
	player = ply;
	type = tp;
	lx = l;
	rx = l + CARDLEN - 1;
}

void Card::cDraw(wchar_t* screen)
{
	switch (type)
	{
	case 0:
		if (!player) {
			wsprintf(&screen[1 * nScreenWidth + lx + 2], L"       P    ");
			wsprintf(&screen[2 * nScreenWidth + lx + 2], L" $    _|_   ");
			wsprintf(&screen[3 * nScreenWidth + lx + 2], L" |  _|_0_|_ ");
			wsprintf(&screen[4 * nScreenWidth + lx + 2], L"/*\\| [] [] |");
		}
		else {
			wsprintf(&screen[47 * nScreenWidth + lx + 2], L"    P       ");
			wsprintf(&screen[48 * nScreenWidth + lx + 2], L"   _|_    $ ");
			wsprintf(&screen[49 * nScreenWidth + lx + 2], L" _|_0_|_  | ");
			wsprintf(&screen[50 * nScreenWidth + lx + 2], L"| [] [] |/*\\");
		}
		break;

	case 1:
		if (!player) {
			wsprintf(&screen[2 * nScreenWidth + lx + 4], L"  o   ");
			wsprintf(&screen[3 * nScreenWidth + lx + 4], L"< H --");
			wsprintf(&screen[4 * nScreenWidth + lx + 4], L" / \\  ");
		}
		else {
			wsprintf(&screen[48 * nScreenWidth + lx + 4], L"   o  ");
			wsprintf(&screen[49 * nScreenWidth + lx + 4], L"-- H >");
			wsprintf(&screen[50 * nScreenWidth + lx + 4], L"  / \\ ");
		}
		break;
	case 2:
		if (!player) {
			wsprintf(&screen[1 * nScreenWidth + lx + 2], L"    ___ __ ");
			wsprintf(&screen[2 * nScreenWidth + lx + 2], L" __/___\\__ ");
			wsprintf(&screen[3 * nScreenWidth + lx + 2], L"/_________\\");
			wsprintf(&screen[4 * nScreenWidth + lx + 2], L"o o o o o o");
		}
		else {
			wsprintf(&screen[47 * nScreenWidth + lx + 2], L" __ ___    ");
			wsprintf(&screen[48 * nScreenWidth + lx + 2], L" __/___\\__ ");
			wsprintf(&screen[49 * nScreenWidth + lx + 2], L"/_________\\");
			wsprintf(&screen[50 * nScreenWidth + lx + 2], L"o o o o o o");
		}
		break;
	case 3:
		if (!player) {
			wsprintf(&screen[1 * nScreenWidth + lx + 2], L"  // // _  ");
			wsprintf(&screen[2 * nScreenWidth + lx + 2], L" //_//_|_\\ ");
			wsprintf(&screen[3 * nScreenWidth + lx + 2], L"|_________\\");
			wsprintf(&screen[4 * nScreenWidth + lx + 2], L" O      O  ");
		}
		else {
			wsprintf(&screen[47 * nScreenWidth + lx + 2], L"  _ \\\\ \\\\  ");
			wsprintf(&screen[48 * nScreenWidth + lx + 2], L" /_|_\\\\_\\\\ ");
			wsprintf(&screen[49 * nScreenWidth + lx + 2], L"/_________|");
			wsprintf(&screen[50 * nScreenWidth + lx + 2], L" O      O  ");
		}
		break;
	case 4:
		if (!player) {
			wsprintf(&screen[2 * nScreenWidth + lx + 2], L"  _     _  ");
			wsprintf(&screen[3 * nScreenWidth + lx + 2], L" / \\___/ \\ ");
			wsprintf(&screen[4 * nScreenWidth + lx + 2], L"/ #|   |# \\");
		}
		else {
			wsprintf(&screen[48 * nScreenWidth + lx + 2], L"  _     _  ");
			wsprintf(&screen[49 * nScreenWidth + lx + 2], L" / \\___/ \\ ");
			wsprintf(&screen[50 * nScreenWidth + lx + 2], L"/ #|   |# \\");
		}
		break;
	case 5:
		if (!player) {
			wsprintf(&screen[1 * nScreenWidth + lx + 1], L"        $$$  ");
			wsprintf(&screen[2 * nScreenWidth + lx + 1], L"        ___  ");
			wsprintf(&screen[3 * nScreenWidth + lx + 1], L" ______/. .\\ ");
			wsprintf(&screen[4 * nScreenWidth + lx + 1], L"|[]00 /  .  \\");
		}
		else {
			wsprintf(&screen[47 * nScreenWidth + lx + 1], L"  $$$       ");
			wsprintf(&screen[48 * nScreenWidth + lx + 1], L"  ___       ");
			wsprintf(&screen[49 * nScreenWidth + lx + 1], L" /. .\\______ ");
			wsprintf(&screen[50 * nScreenWidth + lx + 1], L"/  .  \\ 00[]|");
		}
		break;
	case 6:
		if (!player) {
			wsprintf(&screen[1 * nScreenWidth + lx + 4], L"   (!) ");
			wsprintf(&screen[2 * nScreenWidth + lx + 4], L" ___|_ ");
			wsprintf(&screen[3 * nScreenWidth + lx + 4], L"|_____|");
			wsprintf(&screen[4 * nScreenWidth + lx + 4], L" o   o ");
		}
		else {
			wsprintf(&screen[47 * nScreenWidth + lx + 4], L" (!)   ");
			wsprintf(&screen[48 * nScreenWidth + lx + 4], L" _|___ ");
			wsprintf(&screen[49 * nScreenWidth + lx + 4], L"|_____|");
			wsprintf(&screen[50 * nScreenWidth + lx + 4], L" o   o ");
		}
		break;
	case 7:
		if (!player) {
			wsprintf(&screen[1 * nScreenWidth + lx + 2], L"  [_]======>");
			wsprintf(&screen[2 * nScreenWidth + lx + 2], L" /___\\___   ");
			wsprintf(&screen[3 * nScreenWidth + lx + 2], L"[--------)  ");
			wsprintf(&screen[4 * nScreenWidth + lx + 2], L" O  OOOOO   ");
		}
		else {
			wsprintf(&screen[47 * nScreenWidth + lx + 1], L"<======[_]  ");
			wsprintf(&screen[48 * nScreenWidth + lx + 1], L"   ___/___\\ ");
			wsprintf(&screen[49 * nScreenWidth + lx + 1], L"  (--------]");
			wsprintf(&screen[50 * nScreenWidth + lx + 1], L"   OOOOO  O ");
		}
		break;
	case 8:
		if (!player) {
			wsprintf(&screen[1 * nScreenWidth + lx + 4], L"((*A*))");
			wsprintf(&screen[2 * nScreenWidth + lx + 4], L" __|__ ");
			wsprintf(&screen[3 * nScreenWidth + lx + 4], L"|__+__|");
			wsprintf(&screen[4 * nScreenWidth + lx + 4], L"O     O");
		}
		else {
			wsprintf(&screen[47 * nScreenWidth + lx + 4], L"((*A*))");
			wsprintf(&screen[48 * nScreenWidth + lx + 4], L" __|__ ");
			wsprintf(&screen[49 * nScreenWidth + lx + 4], L"|__+__|");
			wsprintf(&screen[50 * nScreenWidth + lx + 4], L"O     O");
		}

		break;
	case 9:
		if (!player) {
			wsprintf(&screen[1 * nScreenWidth + lx + 3], L"    ==={-");
			wsprintf(&screen[2 * nScreenWidth + lx + 3], L"_[]_||___");
			wsprintf(&screen[3 * nScreenWidth + lx + 3], L"\\_______/");
			wsprintf(&screen[4 * nScreenWidth + lx + 3], L" oo   oo ");
		}
		else {
			wsprintf(&screen[47 * nScreenWidth + lx + 3], L"-}===    ");
			wsprintf(&screen[48 * nScreenWidth + lx + 3], L"___||_[]_");
			wsprintf(&screen[49 * nScreenWidth + lx + 3], L"\\_______/");
			wsprintf(&screen[50 * nScreenWidth + lx + 3], L" oo   oo ");
		}
		break;
	case 10:
		if (!player) {
			wsprintf(&screen[2 * nScreenWidth + lx + 4], L"  o   ");
			wsprintf(&screen[3 * nScreenWidth + lx + 4], L"[]H--=");
			wsprintf(&screen[4 * nScreenWidth + lx + 4], L" / \\  ");
		}
		else {
			wsprintf(&screen[48 * nScreenWidth + lx + 4], L"   o  ");
			wsprintf(&screen[49 * nScreenWidth + lx + 4], L"=--H[]");
			wsprintf(&screen[50 * nScreenWidth + lx + 4], L"  / \\ ");
		}
		break;
	case 11:
		if (!player) {
			wsprintf(&screen[2 * nScreenWidth + lx + 5], L">");
			wsprintf(&screen[3 * nScreenWidth + lx + 2], L"-<-=====>>");
			wsprintf(&screen[4 * nScreenWidth + lx + 5], L">");
		}
		else {
			wsprintf(&screen[48 * nScreenWidth + lx + 8], L"<");
			wsprintf(&screen[49 * nScreenWidth + lx + 2], L"<<=====->-");
			wsprintf(&screen[50 * nScreenWidth + lx + 8], L"<");
		}
		break;
	case 12:
		if (!player) {
			wsprintf(&screen[1 * nScreenWidth + lx + 4], L" _   _ ");
			wsprintf(&screen[2 * nScreenWidth + lx + 4], L"\\ / \\ /");
			wsprintf(&screen[3 * nScreenWidth + lx + 4], L"   O   ");
			wsprintf(&screen[4 * nScreenWidth + lx + 4], L"  /_\\  ");
		}
		else {
			wsprintf(&screen[47 * nScreenWidth + lx + 4], L" _   _ ");
			wsprintf(&screen[48 * nScreenWidth + lx + 4], L"\\ / \\ /");
			wsprintf(&screen[49 * nScreenWidth + lx + 4], L"   O   ");
			wsprintf(&screen[50 * nScreenWidth + lx + 4], L"  /_\\  ");
		}
		break;
	case 13:
		if (!player) {
			wsprintf(&screen[2 * nScreenWidth + lx + 3], L"   * *   ");
			wsprintf(&screen[3 * nScreenWidth + lx + 3], L"( (EMP) )");
			wsprintf(&screen[4 * nScreenWidth + lx + 3], L"   * *   ");
		}
		else {
			wsprintf(&screen[48 * nScreenWidth + lx + 3], L"   * *   ");
			wsprintf(&screen[49 * nScreenWidth + lx + 3], L"( (EMP) )");
			wsprintf(&screen[50 * nScreenWidth + lx + 3], L"   * *   ");
		}
		break;

	case 14:
		if (!player) {
			wsprintf(&screen[2 * nScreenWidth + lx + 2], L"=\\       /=");
			wsprintf(&screen[3 * nScreenWidth + lx + 2], L" ||=====|| ");
			wsprintf(&screen[4 * nScreenWidth + lx + 2], L"=/       \\=");
		}
		else {
			wsprintf(&screen[48 * nScreenWidth + lx + 2], L"=\\       /=");
			wsprintf(&screen[49 * nScreenWidth + lx + 2], L" ||=====|| ");
			wsprintf(&screen[50 * nScreenWidth + lx + 2], L"=/       \\=");
		}
		break;

	case 15:
		if (!player) {
			wsprintf(&screen[2 * nScreenWidth + lx + 6], L"_?_");
			wsprintf(&screen[3 * nScreenWidth + lx + 5], L"[   ]");
		}
		else {
			wsprintf(&screen[48 * nScreenWidth + lx + 6], L"_?_");
			wsprintf(&screen[49 * nScreenWidth + lx + 5], L"[   ]");
		}
		break;
	}
	if (!player) {
		wsprintf(&screen[nScreenWidth + lx + 1], L"%d", mineral);
		wsprintf(&screen[nScreenWidth + lx + 13], L"%d", mineral);
	}
	else {
		wsprintf(&screen[47 * nScreenWidth + lx + 1], L"%d", mineral);
		wsprintf(&screen[47 * nScreenWidth + lx + 13], L"%d", mineral);
	}

	if (player == 0) {
		for (int i = lx; i <= rx; i++)
		{
			screen[i] = L'~';
			screen[5 * nScreenWidth + i] = L'~';
		}
		for (int i = 1; i <= 4; i++)
		{
			screen[i * nScreenWidth + lx] = L'|';
			screen[i * nScreenWidth + rx] = L'|';
		}
	}
	else {
		for (int i = lx; i <= rx; i++)
		{
			screen[46 * nScreenWidth + i] = L'~';
			screen[51 * nScreenWidth + i] = L'~';
		}
		for (int i = 47; i <= 50; i++)
		{
			screen[i * nScreenWidth + lx] = L'|';
			screen[i * nScreenWidth + rx] = L'|';
		}
	}
}

int Card::getPlayer()
{
	return player;
}

int Card::getType()
{
	return type;
}

int Card::getLx()
{
	return lx;
}

int Card::getMineral()
{
	return mineral;
}

void PrintCard(wchar_t* screen, int num, int player, int lx, int y)
{
	switch (num)
	{
	case 0:
		if (!player) {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 2], L"       P    ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L" $    _|_   ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L" |  _|_0_|_ ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L"/*\\| [] [] |");
		}
		else {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 2], L"    P       ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L"   _|_    $ ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L" _|_0_|_  | ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L"| [] [] |/*\\");
		}
		break;

	case 1:
		if (!player) {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 4], L"  o   ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 4], L"< H --");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 4], L" / \\  ");
		}
		else {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 4], L"   o  ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 4], L"-- H >");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 4], L"  / \\ ");
		}
		break;
	case 2:
		if (!player) {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 2], L"    ___ __ ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L" __/___\\__ ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L"/_________\\");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L"o o o o o o");
		}
		else {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 2], L" __ ___    ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L" __/___\\__ ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L"/_________\\");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L"o o o o o o");
		}
		break;
	case 3:
		if (!player) {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 2], L"  // // _  ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L" //_//_|_\\ ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L"|_________\\");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L" O      O  ");
		}
		else {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 2], L"  _ \\\\ \\\\  ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L" /_|_\\\\_\\\\ ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L"/_________|");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L" O      O  ");
		}
		break;
	case 4:
		if (!player) {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L"  _     _  ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L" / \\___/ \\ ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L"/ #|   |# \\");
		}
		else {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L"  _     _  ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L" / \\___/ \\ ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L"/ #|   |# \\");
		}
		break;
	case 5:
		if (!player) {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 1], L"        $$$  ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 1], L"        ___  ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 1], L" ______/. .\\ ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 1], L"|[]00 /  .  \\");
		}
		else {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 1], L"  $$$       ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 1], L"  ___       ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 1], L" /. .\\______ ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 1], L"/  .  \\ 00[]|");
		}
		break;
	case 6:
		if (!player) {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 4], L"   (!) ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 4], L" ___|_ ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 4], L"|_____|");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 4], L" o   o ");
		}
		else {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 4], L" (!)   ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 4], L" _|___ ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 4], L"|_____|");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 4], L" o   o ");
		}
		break;
	case 7:
		if (!player) {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 2], L"  [_]======>");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L" /___\\___   ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L"[--------)  ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L" O  OOOOO   ");
		}
		else {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 1], L"<======[_]  ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 1], L"   ___/___\\ ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 1], L"  (--------]");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 1], L"   OOOOO  O ");
		}
		break;
	case 8:
		if (!player) {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 4], L"((*A*))");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 4], L" __|__ ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 4], L"|__+__|");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 4], L"O     O");
		}
		else {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 4], L"((*A*))");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 4], L" __|__ ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 4], L"|__+__|");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 4], L"O     O");
		}

		break;
	case 9:
		if (!player) {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 3], L"    ==={-");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 3], L"_[]_||___");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 3], L"\\_______/");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 3], L" oo   oo ");
		}
		else {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 3], L"-}===    ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 3], L"___||_[]_");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 3], L"\\_______/");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 3], L" oo   oo ");
		}
		break;
	case 10:
		if (!player) {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 4], L"  o   ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 4], L"[]H--=");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 4], L" / \\  ");
		}
		else {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 4], L"   o  ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 4], L"=--H[]");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 4], L"  / \\ ");
		}
		break;
	case 11:
		if (!player) {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 5], L">");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L"-<-=====>>");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 5], L">");
		}
		else {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 8], L"<");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L"<<=====->-");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 8], L"<");
		}
		break;
	case 12:
		if (!player) {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 4], L" _   _ ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 4], L"\\ / \\ /");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 4], L"   O   ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 4], L"  /_\\  ");
		}
		else {
			wsprintf(&screen[(y + 1) * nScreenWidth + lx + 4], L" _   _ ");
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 4], L"\\ / \\ /");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 4], L"   O   ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 4], L"  /_\\  ");
		}
		break;
	case 13:
		if (!player) {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 3], L"   * *   ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 3], L"( (EMP) )");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 3], L"   * *   ");
		}
		else {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 3], L"   * *   ");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 3], L"( (EMP) )");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 3], L"   * *   ");
		}
		break;

	case 14:
		if (!player) {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L"=\\       /=");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L" ||=====|| ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L"=/       \\=");
		}
		else {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 2], L"=\\       /=");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 2], L" ||=====|| ");
			wsprintf(&screen[(y + 4) * nScreenWidth + lx + 2], L"=/       \\=");
		}
		break;

	case 15:
		if (!player) {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 6], L"_?_");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 5], L"[   ]");
		}
		else {
			wsprintf(&screen[(y + 2) * nScreenWidth + lx + 6], L"_?_");
			wsprintf(&screen[(y + 3) * nScreenWidth + lx + 5], L"[   ]");
		}
		break;
	}
	wsprintf(&screen[(y + 1) * nScreenWidth + lx + 1], L"%d", CardMineral[num]);
	wsprintf(&screen[(y + 1) * nScreenWidth + lx + 13], L"%d", CardMineral[num]);
}
/*
   _   _ 
  \ / \ /
     O   
    /_\  


	   ||   
	<==||==>
	   ||   
       ||   

	  =\       /=
	   ||=====|| 
	  =/       \=
	       * *   
	    ( (EMP) )
		   * *
*/  