/*
 * Copyright (C) 2013-2015 DeathCore <http://www.noffearrdeathproject.net/>
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

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Chat.h"
#include "Common.h"
#include "Language.h"
#include "CellImpl.h"
#include "GridNotifiers.h"

class playercommands : public CommandScript
{
public:
    playercommands() : CommandScript("playercommands") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand playerCommandTable[] =

        {
	
		{ "global",	    rbac::RBAC_PERM_COMMAND_MALL,     true, &HandlePlayerMallCommand,         "", NULL },
        { NULL,             0,                     false, NULL,                                          "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "shop",	    rbac::RBAC_PERM_COMMAND_MALL,   true, NULL,      "",  playerCommandTable},
	       { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }


		static bool HandlePlayerMallCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();

        if (me->IsInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if (me->IsInFlight())
        {
            me->GetMotionMaster()->MovementExpired();
            me->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            me->SaveRecallPosition();

		me->TeleportTo(0, -1818.969971f, -4149.790039f, 31.984400f, 3.091680f); // MapId, X, Y, Z, O
                return true;
    }

};

void AddSC_playercommands()
{
    new playercommands();
}