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

class vipcommands : public CommandScript
{
public:
    vipcommands() : CommandScript("vipcommands") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand vipCommandTable[] =

        {
		{ "buffs",           rbac::RBAC_PERM_COMMAND_VIP,  false, &HandleBuffsCommand,		"", NULL },
		//{ "sala",	    rbac::RBAC_PERM_COMMAND_VIP,     true, &HandleVipMallCommand,         "", NULL },
        { "mudar-raça",    rbac::RBAC_PERM_COMMAND_VIP,  false, &HandleChangeRaceCommand,             "", NULL },
	    { "mudar-facção",	rbac::RBAC_PERM_COMMAND_VIP,  false, &HandleChangeFactionCommand,		"", NULL },
	    { "mudar-visual",	rbac::RBAC_PERM_COMMAND_VIP,  false, &HandleCustomizeCommand,		"", NULL },
        { NULL,             0,                     false, NULL,                                          "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "vip",	    rbac::RBAC_PERM_COMMAND_VIP,   true, NULL,      "",  vipCommandTable},
	       { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

static bool HandleActivateCommand(ChatHandler * handler, const char * args)
{
	Player* player = handler->GetSession()->GetPlayer();

	if(player->GetSession()->GetSecurity() >= 1)
	{
		handler->PSendSysMessage("Você já é um Jogador VIP");
		handler->SetSentErrorMessage(true);
		return false;
	}

	if(player->HasItemCount(313370, 1, false)) // Token ID, Count.
	{
                  PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_ACCESS);
                  stmt->setUInt32(0, player->GetSession()->GetAccountId());
                  stmt->setUInt8(1, 1);
                  stmt->setInt32(2, 1);
	         LoginDatabase.Execute(stmt);
		player->DestroyItemCount(313370, 1, true, false); // Token ID, Count.
		handler->PSendSysMessage("Você agora é um Jogador VIP, para ativa-lo relogue e entre no jogo novamente");
		return true;
	}
	return true;
}

static bool HandleBuffsCommand(ChatHandler * handler, const char * args)
    {
        Player * pl = handler->GetSession()->GetPlayer();
		if(pl->IsInCombat())
		{
			pl->GetSession()->SendNotification("Você não pode usar isso em combate!!");
			return false;
		}
		if(pl->InArena())
		{
			pl->GetSession()->SendNotification("Você não pode usar isso em uma arena!");
			return false;
		}
		else
		pl->AddAura(21562, pl);
		pl->AddAura(20217, pl);
		pl->AddAura(6673, pl);
		pl->AddAura(57330, pl);
		pl->AddAura(3714, pl);
		pl->AddAura(467, pl);
		pl->AddAura(469, pl);
		pl->AddAura(70233, pl);
		pl->AddAura(19740, pl);
		pl->AddAura(70234, pl);
		pl->AddAura(70242, pl);
		pl->AddAura(70243, pl);
		pl->AddAura(33377, pl);
		pl->AddAura(30562, pl);
		pl->AddAura(30550, pl);
		pl->AddAura(70235, pl);
		pl->AddAura(70244, pl);
		pl->AddAura(43818, pl);
		pl->AddAura(43822, pl);
		pl->AddAura(43816, pl);
		pl->AddAura(42138, pl);
		pl->AddAura(15366, pl);
		pl->AddAura(5862, pl);
		handler->PSendSysMessage("|cffB400B4[|cffFFA500BUFF V.I.P|cffB400B4] |cffFF0000Você está quase Imortal!");
		return true;

    }


static bool HandlevipCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();

            me->Say("Comandos V.I.P?", LANG_UNIVERSAL);
            return true;
    }

static bool HandleChangeRaceCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
		handler->PSendSysMessage("Relogue para mudar sua raça!");
        return true;
    }

static bool HandleChangeFactionCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
		handler->PSendSysMessage("Relogue para mudar sua facção!");
        return true;
    }

static bool HandleCustomizeCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
		handler->PSendSysMessage("Relogue para mudar o seu visual!");
        return true;
    }

		static bool HandleVipMallCommand(ChatHandler* handler, const char* args)
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

		me->TeleportTo(0, 11215.794922f, -1793.200195f, -28.392117f, 1.654364f); // MapId, X, Y, Z, O
                return true;
    }

};

void AddSC_vipcommands()
{
    new vipcommands();
}