/*
	Copyright (C) 2009-2013 DeSmuME team

	This file is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	This file is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with the this software.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __SLOT2_H__
#define __SLOT_H__

#include "common.h"
#include "types.h"
#include "debug.h"

class Slot2Info
{
public:
	virtual const char* name() const = 0;
	virtual const char* descr() const  = 0;
};

class Slot2InfoSimple : public Slot2Info
{
public:
	Slot2InfoSimple(const char* _name, const char* _descr)
		: mName(_name)
		, mDescr(_descr)
	{
	}
	virtual const char* name() const { return mName; }
	virtual const char* descr() const { return mDescr; }
private:
	const char* mName, *mDescr;
};

class ISlot2Interface
{
public:
	//called to get info about device (description)
	virtual Slot2Info const* info() = 0;

	//called once when the emulator starts up, or when the device springs into existence
	virtual bool init() { return true; }
	
	//called when the emulator connects the device
	virtual void connect() { }

	//called when the emulator disconnects the device
	virtual void disconnect() { }
	
	//called when the emulator shuts down, or when the device disappears from existence
	virtual void shutdown() { }

	virtual void writeByte(u8 PROCNUM, u32 addr, u8 val) {};
	virtual void writeWord(u8 PROCNUM, u32 addr, u16 val) {};
	virtual void writeLong(u8 PROCNUM, u32 addr, u32 val) {};

	virtual u8	readByte(u8 PROCNUM, u32 addr) { return 0xFF; };
	virtual u16	readWord(u8 PROCNUM, u32 addr) { return 0xFFFF; };
	virtual u32	readLong(u8 PROCNUM, u32 addr) { return 0xFFFFFFFF; };

	virtual void savestate(EMUFILE* os) {}

	virtual void loadstate(EMUFILE* is) {}
}; 

typedef ISlot2Interface* TISlot2InterfaceConstructor();

enum NDS_SLOT2_TYPE
{
	NDS_SLOT2_NONE,
	NDS_SLOT2_AUTO,
	NDS_SLOT2_CFLASH,		// compact flash
	NDS_SLOT2_RUMBLEPAK,	// rumble pack
	NDS_SLOT2_GBACART,		// GBA cartrindge in slot
	NDS_SLOT2_GUITARGRIP,	// Guitar Grip
	NDS_SLOT2_EXPMEMORY,	// Memory Expansion 
	NDS_SLOT2_EASYPIANO,	// Easy Piano
	NDS_SLOT2_PADDLE,
	NDS_SLOT2_PASSME,		// PassME
	NDS_SLOT2_COUNT			// use for counter addons - MUST TO BE LAST!!!
};

extern ISlot2Interface* slot2_device;						//the current slot2 device instance
extern ISlot2Interface* slot2_List[NDS_SLOT2_COUNT];

void slot2_Init();
bool slot2_Connect();
void slot2_Disconnect();
void slot2_Shutdown();
void slot2_Savestate(EMUFILE* os);
void slot2_Loadstate(EMUFILE* is);

//just disconnects and reconnects the device. ideally, the disconnection and connection would be called with sensible timing
void slot2_Reset();

//change the current device
bool slot2_Change(NDS_SLOT2_TYPE type);

//check on the current device
NDS_SLOT2_TYPE slot2_GetCurrentType();

template <u8 PROCNUM, typename T>
bool slot2_write(u32 addr, T val);

template <u8 PROCNUM, typename T>
bool slot2_read(u32 addr, T &val);


// =================================================================================
extern char GBAgameName[MAX_PATH];					// file name for GBA game (rom)
extern void (*FeedbackON)(BOOL enable);				// feedback on/off

enum ADDON_CFLASH_MODE
{
	ADDON_CFLASH_MODE_Path, ADDON_CFLASH_MODE_File, ADDON_CFLASH_MODE_RomPath
};

extern ADDON_CFLASH_MODE CFlash_Mode;
extern std::string CFlash_Path;
inline bool CFlash_IsUsingPath() { return CFlash_Mode==ADDON_CFLASH_MODE_Path || CFlash_Mode==ADDON_CFLASH_MODE_RomPath; }

extern void guitarGrip_setKey(bool green, bool red, bool yellow, bool blue); // Guitar grip keys
extern void piano_setKey(bool c, bool cs, bool d, bool ds, bool e, bool f, bool fs, bool g, bool gs, bool a, bool as, bool b, bool hic); //piano keys
#endif //__SLOT_H__