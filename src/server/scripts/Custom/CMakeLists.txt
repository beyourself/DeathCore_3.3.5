# Copyright (C) 2013-2015 DeathCore <http://www.noffearrdeathproject.net/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# file(GLOB_RECURSE sources_Custom Custom/*.cpp Custom/*.h)

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
#  ${sources_Custom}
Custom/anti_divulgador.cpp
Custom/anti_farm.cpp
Custom/anuncio_de_boss.cpp
Custom/comando_free_buff.cpp
Custom/comando_shop.cpp
Custom/comando_vip.cpp
Custom/duel_reset.cpp
Custom/ler_spell_level.cpp
Custom/morphs_npc.cpp
Custom/npc_ferramentas_personagem.cpp
Custom/npc_visualweapon.cpp
Custom/sistema_de_gold.cpp
Custom/token_vip.cpp
Custom/top_5_matadores.cpp
Custom/vendedor_vip.cpp
)

message("  -> Prepared: Custom")
