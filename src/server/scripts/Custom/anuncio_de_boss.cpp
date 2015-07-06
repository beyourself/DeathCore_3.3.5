/*
* Copyright (C) 2013-2015 DeathCore <http://www.noffearrdeathproject.net/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptPCH.h"

class Boss_Announcer : public PlayerScript
{
public:
	Boss_Announcer() : PlayerScript("Boss_Announcer") {}

	void OnCreatureKill(Player* player, Creature* boss)
	{

		if (boss->isWorldBoss())
		{
			if (player->getGender() == GENDER_MALE)
			{
				char msg[250];
				snprintf(msg, 250, "|CFF7BBEF7[Boss Announcer]|r:|cffff0000 %s |r e seu grupo matou o Boss |CFF18BE00[%s]|r !!!", player->GetName(), boss->GetName());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
			}
			else
			{
				char msg[250];
				snprintf(msg, 250, "|CFF7BBEF7[Boss Announcer]|r:|cffff0000 %s |r e seu grupo matou o Boss |CFF18BE00[%s]|r !!!", player->GetName(), boss->GetName());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
			}
		}
	}
};

void AddSC_Boss_Announcer()
{
	new Boss_Announcer;
}