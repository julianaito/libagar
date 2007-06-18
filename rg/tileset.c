/*
 * Copyright (c) 2004-2007 Hypertriton, Inc. <http://hypertriton.com/>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <core/core.h>
#include <core/view.h>
#include <core/typesw.h>

#include <gui/window.h>
#include <gui/box.h>
#include <gui/tlist.h>
#include <gui/button.h>
#include <gui/textbox.h>
#include <gui/mspinbutton.h>
#include <gui/checkbox.h>
#include <gui/menu.h>
#include <gui/notebook.h>
#include <gui/radio.h>
#include <gui/combo.h>

#include "tileset.h"

#include <ctype.h>
#include <string.h>

const AG_ObjectOps rgTilesetOps = {
	"RG_Tileset",
	sizeof(RG_Tileset),
	{ 8, 0 },
	RG_TilesetInit,
	RG_TilesetReinit,
	RG_TilesetDestroy,
	RG_TilesetLoad,
	RG_TilesetSave,
#ifdef EDITION
	RG_TilesetEdit
#else
	NULL
#endif
};

extern const RG_FeatureOps rgFillOps;
extern const RG_FeatureOps rgSketchProjOps;

const RG_FeatureOps *feature_tbl[] = {
	&rgFillOps,
	&rgSketchProjOps,
	NULL
};
extern const char *rgTileSnapModes[];

void
RG_InitSubsystem(void)
{
	AG_RegisterType(&rgTilesetOps, TILESET_ICON);
}

void
RG_TilesetInit(void *obj, const char *name)
{
	RG_Tileset *ts = obj;

	AG_ObjectInit(ts, name, &rgTilesetOps);

	/* Restart the graphical editor on load. */
	AGOBJECT(ts)->flags |= AG_OBJECT_REOPEN_ONLOAD;

	AG_MutexInitRecursive(&ts->lock);
	TAILQ_INIT(&ts->tiles);
	TAILQ_INIT(&ts->sketches);
	TAILQ_INIT(&ts->pixmaps);
	TAILQ_INIT(&ts->features);
	TAILQ_INIT(&ts->animations);
	TAILQ_INIT(&ts->textures);

	ts->icon = SDL_CreateRGBSurface(
	    SDL_SWSURFACE|SDL_SRCALPHA|SDL_SRCCOLORKEY,
	    32, 32, agSurfaceFmt->BitsPerPixel,
	    agSurfaceFmt->Rmask,
	    agSurfaceFmt->Gmask,
	    agSurfaceFmt->Bmask,
	    agSurfaceFmt->Amask);

	if (ts->icon == NULL) {
		fatal("SDL_CreateRGBSurface: %s", SDL_GetError());
	}
	ts->fmt = ts->icon->format;
	ts->flags = 0;
	ts->template[0] = '\0';
	ts->tiletbl = NULL;
	ts->ntiletbl = 0;
	ts->animtbl = NULL;
	ts->nanimtbl = 0;
}

void
RG_TilesetReinit(void *obj)
{
	RG_Tileset *ts = obj;
	RG_Tile *t, *nt;
	RG_Sketch *sk, *nsk;
	RG_Pixmap *px, *npx;
	RG_Feature *ft, *nft;
	RG_Anim *ani, *nani;
	RG_Texture *tex, *ntex;
	int i;

	AG_MutexLock(&ts->lock);
	
	for (t = TAILQ_FIRST(&ts->tiles);
	     t != TAILQ_END(&ts->tiles);
	     t = nt) {
		nt = TAILQ_NEXT(t, tiles);
		RG_TileDestroy(t);
		Free(t, M_RG);
	}
	for (sk = TAILQ_FIRST(&ts->sketches);
	     sk != TAILQ_END(&ts->sketches);
	     sk = nsk) {
		nsk = TAILQ_NEXT(sk, sketches);
		RG_SketchDestroy(sk);
		Free(sk, M_RG);
	}
	for (px = TAILQ_FIRST(&ts->pixmaps);
	     px != TAILQ_END(&ts->pixmaps);
	     px = npx) {
		npx = TAILQ_NEXT(px, pixmaps);
		RG_PixmapDestroy(px);
		Free(px, M_RG);
	}
	for (ft = TAILQ_FIRST(&ts->features);
	     ft != TAILQ_END(&ts->features);
	     ft = nft) {
		nft = TAILQ_NEXT(ft, features);
		AG_FeatureDestroy(ft);
		Free(ft, M_RG);
	}
	for (ani = TAILQ_FIRST(&ts->animations);
	     ani != TAILQ_END(&ts->animations);
	     ani = nani) {
		nani = TAILQ_NEXT(ani, animations);
		RG_AnimDestroy(ani);
		Free(ani, M_RG);
	}
	for (tex = TAILQ_FIRST(&ts->textures);
	     tex != TAILQ_END(&ts->textures);
	     tex = ntex) {
		ntex = TAILQ_NEXT(tex, textures);
		RG_TextureDestroy(tex);
		Free(tex, M_RG);
	}
	
	TAILQ_INIT(&ts->tiles);
	TAILQ_INIT(&ts->sketches);
	TAILQ_INIT(&ts->pixmaps);
	TAILQ_INIT(&ts->features);
	TAILQ_INIT(&ts->animations);
	TAILQ_INIT(&ts->textures);

	Free(ts->tiletbl, M_RG);
	ts->tiletbl = NULL;
	Free(ts->animtbl, M_RG);
	ts->animtbl = NULL;

	ts->ntiletbl = 0;
	ts->nanimtbl = 0;

	AG_MutexUnlock(&ts->lock);
}

void
RG_TilesetDestroy(void *obj)
{
	RG_Tileset *ts = obj;

	AG_MutexDestroy(&ts->lock);
	SDL_FreeSurface(ts->icon);
	Free(ts->tiletbl, M_RG);
	Free(ts->animtbl, M_RG);
}

int
RG_TilesetLoad(void *obj, AG_Netbuf *buf)
{
	RG_Tileset *ts = obj;
	RG_Pixmap *px;
	Uint32 count, i;

	if (AG_ReadVersion(buf, rgTilesetOps.type, &rgTilesetOps.ver, NULL)
	    != 0)
		return (-1);
	
	AG_MutexLock(&ts->lock);
	AG_CopyString(ts->template, buf, sizeof(ts->template));
	ts->flags = AG_ReadUint32(buf);

	/* Load the vectorial sketches. */
	count = AG_ReadUint32(buf);
	for (i = 0; i < count; i++) {
		RG_Sketch *sk;

		sk = Malloc(sizeof(RG_Sketch), M_RG);
		RG_SketchInit(sk, ts, 0);
		if (RG_SketchLoad(sk, buf) == -1) {
			Free(sk, M_RG);
			goto fail;
		}
		TAILQ_INSERT_TAIL(&ts->sketches, sk, sketches);
	}

	/* Load the pixmaps. */
	count = AG_ReadUint32(buf);
	for (i = 0; i < count; i++) {
		RG_Pixmap *px;

		px = Malloc(sizeof(RG_Pixmap), M_RG);
		RG_PixmapInit(px, ts, 0);
		if (RG_PixmapLoad(px, buf) == -1) {
			Free(px, M_RG);
			goto fail;
		}
		TAILQ_INSERT_TAIL(&ts->pixmaps, px, pixmaps);
	}

	/* Load the features. */
	count = AG_ReadUint32(buf);
	for (i = 0; i < count; i++) {
		const RG_FeatureOps **ftops;
		char name[RG_FEATURE_NAME_MAX];
		char type[RG_FEATURE_TYPE_MAX];
		size_t len;
		RG_Feature *ft;
		int flags;
		
		AG_CopyString(name, buf, sizeof(name));
		AG_CopyString(type, buf, sizeof(type));
		flags = (int)AG_ReadUint32(buf);
		len = (size_t)AG_ReadUint32(buf);

		for (ftops = &feature_tbl[0]; *ftops != NULL; ftops++) {
			if (strcmp((*ftops)->type, type) == 0)
				break;
		}
		if (*ftops == NULL) {
			dprintf("%s: unimplemented feature: %s; "
			        "skipping %lu bytes.\n", name, type,
				(Ulong)len);
			AG_NetbufSeek(buf, len-4, SEEK_CUR);
			continue;
		}

		ft = Malloc((*ftops)->len, M_RG);
		ft->ops = *ftops;
		ft->ops->init(ft, ts, flags);
		if (RG_FeatureLoad(ft, buf) == -1) {
			AG_FeatureDestroy(ft);
			Free(ft, M_RG);
			goto fail;
		}
		TAILQ_INSERT_TAIL(&ts->features, ft, features);
	}
	
	/* Load the tiles. */
	count = AG_ReadUint32(buf);
	dprintf("%u tiles\n", count);
	for (i = 0; i < count; i++) {
		char name[RG_TILE_NAME_MAX];
		RG_Tile *t;
		
		t = Malloc(sizeof(RG_Tile), M_RG);
		AG_CopyString(name, buf, sizeof(name));
		RG_TileInit(t, ts, name);

		if (RG_TileLoad(t, buf) == -1) {
			RG_TileDestroy(t);
			Free(t, M_RG);
			goto fail;
		}

		/* Allocate the surface fragments. */
		RG_TileScale(ts, t, t->su->w, t->su->h, t->flags,
		    t->su->format->alpha);
		RG_TileGenerate(t);
		
		TAILQ_INSERT_TAIL(&ts->tiles, t, tiles);
	}

	/* Load the animation information. */
	count = AG_ReadUint32(buf);
	for (i = 0; i < count; i++) {
		char name[RG_ANIMATION_NAME_MAX];
		RG_Anim *ani;
		int flags;
		
		ani = Malloc(sizeof(RG_Anim), M_RG);
		AG_CopyString(name, buf, sizeof(name));
		flags = (int)AG_ReadUint32(buf);
		RG_AnimInit(ani, ts, name, flags);
		if (RG_AnimLoad(ani, buf) == -1) {
			RG_AnimDestroy(ani);
			Free(ani, M_RG);
			goto fail;
		}
		TAILQ_INSERT_TAIL(&ts->animations, ani, animations);
	}
	
	/* Load the textures. */
	count = AG_ReadUint32(buf);
	for (i = 0; i < count; i++) {
		char name[RG_TEXTURE_NAME_MAX];
		RG_Texture *tex;
		
		tex = Malloc(sizeof(RG_Texture), M_RG);
		AG_CopyString(name, buf, sizeof(name));
		RG_TextureInit(tex, ts, name);
		if (RG_TextureLoad(tex, buf) == -1) {
			RG_TextureDestroy(tex);
			Free(tex, M_RG);
			goto fail;
		}
		TAILQ_INSERT_TAIL(&ts->textures, tex, textures);
	}

	/* Load and resolve the static tile and animation mappings. */
	ts->ntiletbl = AG_ReadUint32(buf);
	printf("%s: tiletbl: %u entries\n", AGOBJECT(ts)->name,
	    (Uint)ts->ntiletbl);
	ts->tiletbl = Realloc(ts->tiletbl, ts->ntiletbl*sizeof(RG_Tile *));
	for (i = 0; i < ts->ntiletbl; i++) {
		char name[RG_TILE_NAME_MAX];

		AG_CopyString(name, buf, sizeof(name));
		printf("tile mapping %u: <%s>\n", i, name);
		if (name[0] == '\0') {
			ts->tiletbl[i] = NULL;
		} else {
			if ((ts->tiletbl[i] = RG_TilesetFindTile(ts, name))
			    == NULL) {
				fatal("%s: bad tile mapping: %s (%u)",
				    AGOBJECT(ts)->name, name, (Uint)i);
			}
		}
	}
	ts->nanimtbl = AG_ReadUint32(buf);
	printf("%s: animtbl(%p): %u anims\n", AGOBJECT(ts)->name, ts->animtbl,
	    (Uint)ts->nanimtbl);
	ts->animtbl = Realloc(ts->animtbl, ts->nanimtbl*sizeof(RG_Anim *));
	for (i = 0; i < ts->nanimtbl; i++) {
		char name[RG_ANIMATION_NAME_MAX];

		AG_CopyString(name, buf, sizeof(name));
		printf("anim mapping %u: <%s>\n", i, name);
		if (name[0] == '\0') {
			ts->animtbl[i] = NULL;
		} else {
			if ((ts->animtbl[i] = RG_TilesetFindAnim(ts, name))
			    == NULL) {
				fatal("%s: bad anim mapping: %s (%u)",
				    AGOBJECT(ts)->name, name, (Uint)i);
			}
		}
	}

	/* Resolve the pixmap brush references. */
	TAILQ_FOREACH(px, &ts->pixmaps, pixmaps) {
		struct rg_pixmap_brush *pbr;
		RG_Pixmap *ppx;

		TAILQ_FOREACH(pbr, &px->brushes, brushes) {
			if (pbr->px != NULL) {
				continue;
			}
			TAILQ_FOREACH(ppx, &ts->pixmaps, pixmaps) {
				if (strcmp(pbr->px_name, ppx->name) == 0) {
					pbr->px = ppx;
					pbr->px->nrefs++;
					break;
				}
			}
			if (ppx == NULL)
				fatal("%s: bad pixmap ref", pbr->px_name);
		}
	}
	AG_MutexUnlock(&ts->lock);
	return (0);
fail:
	AG_MutexUnlock(&ts->lock);
	return (-1);
}

int
RG_TilesetSave(void *obj, AG_Netbuf *buf)
{
	RG_Tileset *ts = obj;
	Uint32 count, i;
	off_t offs;
	RG_Sketch *sk;
	RG_Pixmap *px;
	RG_Anim *ani;
	RG_Tile *t;
	RG_Feature *ft;
	RG_Texture *tex;

	AG_WriteVersion(buf, rgTilesetOps.type, &rgTilesetOps.ver);
	AG_MutexLock(&ts->lock);
	AG_WriteString(buf, ts->template);
	AG_WriteUint32(buf, ts->flags);

	/* Save the vectorial sketches. */
	count = 0;
	offs = AG_NetbufTell(buf);
	AG_WriteUint32(buf, 0);
	TAILQ_FOREACH(sk, &ts->sketches, sketches) {
		RG_SketchSave(sk, buf);
		count++;
	}
	AG_PwriteUint32(buf, count, offs);

	/* Save the pixmaps. */
	count = 0;
	offs = AG_NetbufTell(buf);
	AG_WriteUint32(buf, 0);
	TAILQ_FOREACH(px, &ts->pixmaps, pixmaps) {
		RG_PixmapSave(px, buf);
		count++;
	}
	AG_PwriteUint32(buf, count, offs);

	/* Save the features. */
	count = 0;
	offs = AG_NetbufTell(buf);
	AG_WriteUint32(buf, 0);
	TAILQ_FOREACH(ft, &ts->features, features) {
		off_t ftsize_offs;

		AG_WriteString(buf, ft->name);
		AG_WriteString(buf, ft->ops->type);
		AG_WriteUint32(buf, ft->flags);

		/* Encode the size to allow skipping unimplemented features. */
		ftsize_offs = AG_NetbufTell(buf);
		AG_WriteUint32(buf, 0);
		RG_FeatureSave(ft, buf);
		AG_PwriteUint32(buf, AG_NetbufTell(buf) - ftsize_offs,
		    ftsize_offs);

		count++;
	}
	AG_PwriteUint32(buf, count, offs);
	
	/* Save the tiles. */
	count = 0;
	offs = AG_NetbufTell(buf);
	AG_WriteUint32(buf, 0);
	TAILQ_FOREACH(t, &ts->tiles, tiles) {
		RG_TileSave(t, buf);
		count++;
	}
	AG_PwriteUint32(buf, count, offs);
	dprintf("saved %u tiles\n", count);
	
	/* Save the animation information. */
	count = 0;
	offs = AG_NetbufTell(buf);
	AG_WriteUint32(buf, 0);
	TAILQ_FOREACH(ani, &ts->animations, animations) {
		AG_WriteString(buf, ani->name);
		AG_WriteUint32(buf, (Uint32)ani->flags);
		RG_AnimSave(ani, buf);
		count++;
	}
	AG_PwriteUint32(buf, count, offs);
	
	/* Save the textures. */
	count = 0;
	offs = AG_NetbufTell(buf);
	AG_WriteUint32(buf, 0);
	TAILQ_FOREACH(tex, &ts->textures, textures) {
		AG_WriteString(buf, tex->name);
		RG_TextureSave(tex, buf);
		count++;
	}
	AG_PwriteUint32(buf, count, offs);

	/* Save the static tile and animation mappings. */
	AG_WriteUint32(buf, ts->ntiletbl);
	for (i = 0; i < ts->ntiletbl; i++) {
		AG_WriteString(buf, (ts->tiletbl[i]->name != NULL) ?
		                     ts->tiletbl[i]->name : "");
	}
	AG_WriteUint32(buf, ts->nanimtbl);
	for (i = 0; i < ts->nanimtbl; i++) {
		AG_WriteString(buf, (ts->animtbl[i]->name != NULL) ?
		                     ts->animtbl[i]->name : "");
	}

	AG_MutexUnlock(&ts->lock);
	return (0);
}

RG_Tile *
RG_TilesetFindTile(RG_Tileset *ts, const char *name)
{
	RG_Tile *t;

	TAILQ_FOREACH(t, &ts->tiles, tiles) {
		if (strcmp(t->name, name) == 0)
			break;
	}
	if (t == NULL) {
		AG_SetError("%s: unexisting tile", name);
	}
	return (t);
}

RG_Sketch *
RG_TilesetFindSketch(RG_Tileset *ts, const char *name)
{
	RG_Sketch *sk;

	TAILQ_FOREACH(sk, &ts->sketches, sketches) {
		if (strcmp(sk->name, name) == 0)
			break;
	}
	if (sk == NULL) {
		AG_SetError("%s: unexisting sketch", name);
	}
	return (sk);
}

RG_Pixmap *
RG_TilesetFindPixmap(RG_Tileset *ts, const char *name)
{
	RG_Pixmap *px;

	TAILQ_FOREACH(px, &ts->pixmaps, pixmaps) {
		if (strcmp(px->name, name) == 0)
			break;
	}
	if (px == NULL) {
		AG_SetError("%s: unexisting pixmap", name);
	}
	return (px);
}

RG_Pixmap *
RG_TilesetResvPixmap(const char *tsname, const char *pxname)
{
	RG_Pixmap *px;
	RG_Tileset *ts;

	if ((ts = AG_ObjectFind(tsname)) == NULL) {
		AG_SetError("%s: no such tileset", tsname);
		return (NULL);
	}
	if (!AG_ObjectIsClass(ts, "RG_Tileset:*")) {
		AG_SetError("%s: not a tileset", tsname);
		return (NULL);
	}

	TAILQ_FOREACH(px, &ts->pixmaps, pixmaps) {
		if (strcmp(px->name, pxname) == 0)
			break;
	}
	if (px == NULL) {
		AG_SetError("%s has no `%s' pixmap", tsname, pxname);
	}
	return (px);
}

RG_Tile *
RG_TilesetResvTile(const char *tsname, const char *tname)
{
	RG_Tileset *ts;
	RG_Tile *t;

	if ((ts = AG_ObjectFind(tsname)) == NULL) {
		AG_SetError("%s: no such tileset", tsname);
		return (NULL);
	}
	if (!AG_ObjectIsClass(ts, "RG_Tileset:*")) {
		AG_SetError("%s: not a tileset", tsname);
		return (NULL);
	}

	TAILQ_FOREACH(t, &ts->tiles, tiles) {
		if (strcmp(t->name, tname) == 0)
			break;
	}
	if (t == NULL) {
		AG_SetError("%s has no `%s' tile", tsname, tname);
	}
	return (t);
}

RG_Anim *
RG_TilesetFindAnim(RG_Tileset *ts, const char *name)
{
	RG_Anim *ani;

	TAILQ_FOREACH(ani, &ts->animations, animations) {
		if (strcmp(ani->name, name) == 0)
			break;
	}
	if (ani == NULL) {
		AG_SetError("%s: unexisting animation", name);
	}
	return (ani);
}

/* Lookup a tile by ID. */
int
RG_LookupTile(RG_Tileset *ts, Uint32 id, RG_Tile **t)
{
	if (id >= ts->ntiletbl || ts->tiletbl[id] == NULL) {
		AG_SetError("%s: no such tile: %u", AGOBJECT(ts)->name,
		    (Uint)id);
		return (-1);
	}
	*t = ts->tiletbl[id];
	return (0);
}

/* Lookup an animation by ID. */
int
RG_LookupAnim(RG_Tileset *ts, Uint32 id, RG_Anim **anim)
{
	if (id >= ts->nanimtbl || ts->animtbl[id] == NULL) {
		AG_SetError("%s: no such anim: %u", AGOBJECT(ts)->name,
		    (Uint)id);
		return (-1);
	}
	*anim = ts->animtbl[id];
	return (0);
}

#ifdef EDITION

static void
PollGraphics(AG_Event *event)
{
	AG_Tlist *tl = AG_SELF();
	RG_Tileset *ts = AG_PTR(1);
	RG_Pixmap *px;
	RG_Sketch *sk;
	AG_TlistItem *it;

	AG_TlistClear(tl);
	AG_MutexLock(&ts->lock);

	TAILQ_FOREACH(px, &ts->pixmaps, pixmaps) {
		it = AG_TlistAdd(tl, NULL, "%s (%ux%u) [#%u]",
		    px->name, px->su->w, px->su->h, px->nrefs);
		it->p1 = px;
		it->cat = "pixmap";
		AG_TlistSetIcon(tl, it, px->su);
	}
	TAILQ_FOREACH(sk, &ts->sketches, sketches) {
		it = AG_TlistAdd(tl, NULL,
		    "%s (%ux%u %.0f%%) [#%u]", sk->name, sk->vg->su->w,
		    sk->vg->su->h, sk->vg->scale*100.0, sk->nrefs);
		it->cat = "sketch";
		it->p1 = sk;
		AG_TlistSetIcon(tl, it, sk->vg->su);
	}

	AG_MutexUnlock(&ts->lock);
	AG_TlistRestore(tl);
}

static void
PollTextures(AG_Event *event)
{
	AG_Tlist *tl = AG_SELF();
	RG_Tileset *ts = AG_PTR(1);
	RG_Texture *tex;
	AG_TlistItem *it;

	AG_TlistClear(tl);
	AG_MutexLock(&ts->lock);
	TAILQ_FOREACH(tex, &ts->textures, textures) {
		RG_Tile *t;

		if (tex->tileset[0] != '\0' && tex->tile[0] != '\0' &&
		    (t = RG_TilesetResvTile(tex->tileset, tex->tile))
		     != NULL) {
			it = AG_TlistAdd(tl, NULL, "%s (<%s> %ux%u)",
			    tex->name, t->name, t->su->w, t->su->h);
			AG_TlistSetIcon(tl, it, t->su);
		} else {
			it = AG_TlistAdd(tl, NULL, "%s (<%s:%s>?)",
			    tex->name, tex->tileset, tex->tile);
		}
		it->cat = "texture";
		it->p1 = tex;
	}
	AG_MutexUnlock(&ts->lock);
	AG_TlistRestore(tl);
}

static void
PollAnims(AG_Event *event)
{
	AG_Tlist *tl = AG_SELF();
	RG_Tileset *ts = AG_PTR(1);
	RG_Anim *ani;
	AG_TlistItem *it;

	AG_TlistClear(tl);
	AG_MutexLock(&ts->lock);

	TAILQ_FOREACH(ani, &ts->animations, animations) {
		it = AG_TlistAdd(tl, NULL, "%s (%ux%u) [#%u]", ani->name,
		    ani->w, ani->h, ani->nrefs);
		it->p1 = ani;
		it->cat = "anim";
	}
	
	AG_MutexUnlock(&ts->lock);
	AG_TlistRestore(tl);
}

static void
PollTiles(AG_Event *event)
{
	AG_Tlist *tl = AG_SELF();
	RG_Tileset *ts = AG_PTR(1);
	RG_Tile *t;
	AG_TlistItem *it;
	RG_TileElement *tel;

	AG_TlistClear(tl);
	AG_MutexLock(&ts->lock);
	TAILQ_FOREACH(t, &ts->tiles, tiles) {
		it = AG_TlistAdd(tl, NULL, "%s (%ux%u)", t->name,
		    t->su->w, t->su->h);
		it->depth = 0;
		it->cat = "tile";
		it->p1 = t;
		AG_TlistSetIcon(tl, it, t->su);

		if (!TAILQ_EMPTY(&t->elements)) {
			it->flags |= AG_TLIST_HAS_CHILDREN;
			if (!AG_TlistVisibleChildren(tl, it))
				continue;
		}
	
		TAILQ_FOREACH(tel, &t->elements, elements) {
			switch (tel->type) {
			case RG_TILE_FEATURE:
				{
					RG_Feature *ft =
					    tel->tel_feature.ft;
					RG_FeatureSketch *fts;

					it = AG_TlistAdd(tl, AGICON(OBJ_ICON),
					    "%s [%d,%d] %s", ft->name,
					    tel->tel_feature.x,
					    tel->tel_feature.y,
					    tel->visible ? "" : "(invisible)");
					it->depth = 1;
					it->cat = "tile-feature";
					it->p1 = tel;

					TAILQ_FOREACH(fts, &ft->sketches,
					    sketches) {
						it = AG_TlistAdd(tl,
						    AGICON(DRAWING_ICON),
						    "%s [at %d,%d]%s",
						    fts->sk->name,
						    fts->x, fts->y,
						    fts->visible ? "" :
						    "(invisible)");
						it->depth = 2;
						it->cat = "feature-sketch";
						it->p1 = fts;
					}
				}
				break;
			case RG_TILE_PIXMAP:
				{
					RG_Pixmap *px = tel->tel_pixmap.px;

					it = AG_TlistAdd(tl, NULL,
					    "%s (%ux%u)%s", px->name,
					    tel->tel_pixmap.px->su->w,
					    tel->tel_pixmap.px->su->h,
					    tel->visible ? "" : "(invisible)");
					it->depth = 1;
					it->cat = "tile-pixmap";
					it->p1 = tel;
					AG_TlistSetIcon(tl, it, px->su);
				}
				break;
			case RG_TILE_SKETCH:
				{
					RG_Sketch *sk = tel->tel_sketch.sk;

					it = AG_TlistAdd(tl,
					    AGICON(DRAWING_ICON),
					    "%s (%ux%u)%s", sk->name,
					    tel->tel_sketch.sk->vg->su->w,
					    tel->tel_sketch.sk->vg->su->h,
					    tel->visible ? "" : "(invisible)");
					it->depth = 1;
					it->cat = "tile-pixmap";
					it->p1 = tel;
				}
				break;
			}
		}
	}
	AG_TlistRestore(tl);
	AG_MutexUnlock(&ts->lock);
}

static char ins_tile_name[RG_TILE_NAME_MAX];
static char ins_tile_class[RG_TILE_CLASS_MAX];
static char ins_texture_name[RG_TEXTURE_NAME_MAX];
static char ins_anim_name[RG_TILE_NAME_MAX];
static int ins_tile_w = RG_TILESZ;
static int ins_tile_h = RG_TILESZ;
static int ins_alpha = 0;
static int ins_colorkey = 1;
static enum rg_snap_mode ins_snap_mode = RG_SNAP_NONE;

static int
InsertTileMapping(RG_Tileset *ts, RG_Tile *t, Uint32 *id)
{
	Uint32 i;

	/* Try to recycle NULL ids if we reach the threshold. */
	if (ts->ntiletbl >= RG_TILE_ID_MINREUSE) {
		for (i = 0; i < ts->ntiletbl; i++) {
			if (ts->tiletbl[i] != NULL) {
				continue;
			}
			ts->tiletbl[i] = t;
			if (id != NULL) { *id = i; }
			return (0);
		}
	}
	if ((ts->ntiletbl+1) >= RG_TILE_ID_MAX) {
		AG_SetError("Out of tile ID space");
		return (-1);
	}
	ts->tiletbl = Realloc(ts->tiletbl, (ts->ntiletbl+1)*sizeof(RG_Tile *));
	ts->tiletbl[ts->ntiletbl] = t;
	if (id != NULL) { *id = ts->ntiletbl++; }
	return (0);
}

static int
InsertAnimMapping(RG_Tileset *ts, RG_Anim *anim, Uint32 *id)
{
	Uint32 i;

	/* Try to recycle NULL ids if we reach the threshold. */
	if (ts->nanimtbl >= RG_ANIM_ID_MINREUSE) {
		for (i = 0; i < ts->nanimtbl; i++) {
			if (ts->animtbl[i] != NULL) {
				continue;
			}
			ts->animtbl[i] = anim;
			if (id != NULL) { *id = i; }
			return (0);
		}
	}
	if ((ts->nanimtbl+1) >= RG_ANIM_ID_MAX) {
		AG_SetError("Out of anim ID space");
		return (-1);
	}
	ts->animtbl = Realloc(ts->animtbl, (ts->nanimtbl+1)*sizeof(RG_Anim *));
	ts->animtbl[ts->nanimtbl] = anim;
	if (id != NULL) { *id = ts->nanimtbl++; }
	return (0);
}

static __inline__ void
RemoveTileMappings(RG_Tileset *ts, RG_Tile *t)
{
	Uint32 i;

	for (i = 0; i < ts->ntiletbl; i++) {
		if (ts->tiletbl[i] == t)
			ts->tiletbl[i] = NULL;
	}
}

static __inline__ void
RemoveAnimMappings(RG_Tileset *ts, RG_Anim *anim)
{
	Uint32 i;

	for (i = 0; i < ts->nanimtbl; i++) {
		if (ts->animtbl[i] == anim)
			ts->animtbl[i] = NULL;
	}
}

static void
InsertTile(AG_Event *event)
{
	AG_Window *pwin = AG_PTR(1);
	RG_Tileset *ts = AG_PTR(2);
	RG_Tile *t;
	Uint flags = 0;

	if (ins_alpha)		flags |= RG_TILE_SRCALPHA;
	if (ins_colorkey)	flags |= RG_TILE_SRCCOLORKEY;

	if (ins_tile_name[0] == '\0') {
		Uint nameno = 0;
tryname1:
		snprintf(ins_tile_name, sizeof(ins_tile_name), _("Tile #%d"),
		    nameno);
		TAILQ_FOREACH(t, &ts->tiles, tiles) {
			if (strcmp(t->name, ins_tile_name) == 0)
				break;
		}
		if (t != NULL) {
			nameno++;
			goto tryname1;
		}
	} else {
tryname2:
		TAILQ_FOREACH(t, &ts->tiles, tiles) {
			if (strcmp(t->name, ins_tile_name) == 0)
				break;
		}
		if (t != NULL) {
			char *np;
			int num = -1;

			for (np = &ins_tile_name[strlen(ins_tile_name)-1];
			     np > &ins_tile_name[0];
			     np--) {
				if (*np == '#' && *(np+1) != '\0') {
					np++;
					num = atoi(np) + 1;
					snprintf(np, sizeof(ins_tile_name) -
					    (np-ins_tile_name)-1, "%d", num);
					break;
				}
				if (!isdigit(*np)) {
					strlcat(ins_tile_name, "_",
					    sizeof(ins_tile_name));
					break;
				}
			}
			goto tryname2;
		}
	}

	t = Malloc(sizeof(RG_Tile), M_RG);
	RG_TileInit(t, ts, ins_tile_name);
	if (strcmp(ts->template, "Perso") == 0) {
		RG_TileScale(ts, t, 16, 32, flags, SDL_ALPHA_OPAQUE);
		t->xOrig = 8;
		t->yOrig = 31;
		t->snap_mode = RG_SNAP_TO_GRID;
		RG_TILE_LAYER2(t,0,0) = +2;
		RG_TILE_LAYER2(t,0,1) = +1;
		RG_TILE_ATTR2(t,0,1) = RG_TILE_BLOCK;
	} else if (strcmp(ts->template, "Terrain") == 0) {
		RG_TileScale(ts, t, 64, 64, flags, SDL_ALPHA_OPAQUE);
		t->xOrig = 24;
		t->yOrig = 24;
		t->snap_mode = RG_SNAP_TO_GRID;
		RG_TILE_ATTR2(t,0,0) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,1,0) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,2,0) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,3,0) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,0,1) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,0,2) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,0,3) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,3,1) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,3,2) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,3,3) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,1,3) = RG_TILE_BLOCK;
		RG_TILE_ATTR2(t,2,3) = RG_TILE_BLOCK;
	} else {
		RG_TileScale(ts, t, ins_tile_w, ins_tile_h, flags,
		    SDL_ALPHA_OPAQUE);
		t->xOrig = t->su->w/2;
		t->yOrig = t->su->h/2;
		t->snap_mode = ins_snap_mode;
	}
	if (InsertTileMapping(ts, t, &t->main_id) == 0) {
		TAILQ_INSERT_TAIL(&ts->tiles, t, tiles);
	} else {
		RG_TileDestroy(t);
		Free(t, M_RG);
		AG_TextMsg(AG_MSG_ERROR, _("Failed to create item: %s"),
		    AG_GetError());
	}
	ins_tile_name[0] = '\0';
	AG_ViewDetach(pwin);
}

static void
InsertTexture(AG_Event *event)
{
	AG_Window *dlgwin = AG_PTR(1);
	AG_Window *pwin = AG_PTR(2);
	RG_Tileset *ts = AG_PTR(3);
	AG_Window *win;
	RG_Texture *tex;
	Uint flags = 0;

	if (ins_texture_name[0] == '\0') {
		Uint nameno = 0;
tryname1:
		snprintf(ins_texture_name, sizeof(ins_texture_name),
		    _("Texture #%d"), nameno);
		TAILQ_FOREACH(tex, &ts->textures, textures) {
			if (strcmp(tex->name, ins_texture_name) == 0)
				break;
		}
		if (tex != NULL) {
			nameno++;
			goto tryname1;
		}
	} else {
tryname2:
		TAILQ_FOREACH(tex, &ts->textures, textures) {
			if (strcmp(tex->name, ins_texture_name) == 0)
				break;
		}
		if (tex != NULL) {
			char *np;
			int num = -1;

			for (np = &ins_texture_name[strlen(ins_texture_name)-1];
			     np > &ins_texture_name[0];
			     np--) {
				if (*np == '#' && *(np+1) != '\0') {
					np++;
					num = atoi(np) + 1;
					snprintf(np, sizeof(ins_texture_name) -
					    (np-ins_texture_name)-1, "%d", num);
					break;
				}
				if (!isdigit(*np)) {
					strlcat(ins_texture_name, "_",
					    sizeof(ins_texture_name));
					break;
				}
			}
			goto tryname2;
		}
	}

	tex = Malloc(sizeof(RG_Texture), M_RG);
	RG_TextureInit(tex, ts, ins_texture_name);
	TAILQ_INSERT_TAIL(&ts->textures, tex, textures);
	
	ins_texture_name[0] = '\0';
	AG_ViewDetach(dlgwin);
	
	if ((win = RG_TextureEdit(tex)) != NULL) {
		AG_WindowAttach(pwin, win);
		AG_WindowShow(win);
	}
}

static void
InsertAnim(AG_Event *event)
{
	AG_Window *pwin = AG_PTR(1);
	RG_Tileset *ts = AG_PTR(2);
	RG_Anim *ani;
	int flags = 0;

	if (ins_alpha)		flags |= RG_ANIM_SRCALPHA;
	if (ins_colorkey)	flags |= RG_ANIM_SRCCOLORKEY;

	if (ins_anim_name[0] == '\0') {
		Uint nameno = 0;
tryname1:
		snprintf(ins_anim_name, sizeof(ins_anim_name),
		    _("Animation #%d"), nameno);
		TAILQ_FOREACH(ani, &ts->animations, animations) {
			if (strcmp(ani->name, ins_anim_name) == 0)
				break;
		}
		if (ani != NULL) {
			nameno++;
			goto tryname1;
		}
	} else {
tryname2:
		TAILQ_FOREACH(ani, &ts->animations, animations) {
			if (strcmp(ani->name, ins_anim_name) == 0)
				break;
		}
		if (ani != NULL) {
			char *np;
			int num = -1;

			for (np = &ins_anim_name[strlen(ins_anim_name)-1];
			     np > &ins_anim_name[0];
			     np--) {
				if (*np == '#' && *(np+1) != '\0') {
					np++;
					num = atoi(np) + 1;
					snprintf(np, sizeof(ins_anim_name) -
					    (np-ins_anim_name)-1, "%d", num);
					break;
				}
				if (!isdigit(*np)) {
					strlcat(ins_anim_name, "_",
					    sizeof(ins_anim_name));
					break;
				}
			}
			goto tryname2;
		}
	}

	ani = Malloc(sizeof(RG_Anim), M_RG);
	RG_AnimInit(ani, ts, ins_anim_name, flags);
	RG_AnimScale(ani, ins_tile_w, ins_tile_h);
	if (InsertAnimMapping(ts, ani, &ani->main_id) == 0) {
		TAILQ_INSERT_TAIL(&ts->animations, ani, animations);
	} else {
		RG_AnimDestroy(ani);
		Free(ani, M_RG);
		AG_TextMsg(AG_MSG_ERROR, _("Failed to create item: %s"),
		    AG_GetError());
	}
	ins_anim_name[0] = '\0';
	AG_ViewDetach(pwin);
}

static void
InsertTileDlg(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Window *pwin = AG_PTR(2);
	AG_Window *win;
	AG_Box *btnbox;
	AG_Textbox *tb;
	AG_MSpinbutton *msb;
	AG_Checkbox *cb;
	AG_Radio *rad;
	AG_Combo *com;

	if ((win = AG_WindowNewNamed(AG_WINDOW_MODAL|AG_WINDOW_NORESIZE|
	    AG_WINDOW_NOMINIMIZE, "rg-instiledlg")) == NULL) {
		return;
	}
	AG_WindowSetCaption(win, _("Create new tile"));
	
	tb = AG_TextboxNew(win, AG_TEXTBOX_HFILL|AG_TEXTBOX_FOCUS, _("Name: "));
	AG_WidgetBind(tb, "string", AG_WIDGET_STRING, ins_tile_name,
	    sizeof(ins_tile_name));
	AG_SetEvent(tb, "textbox-return", InsertTile, "%p,%p", win, ts);

	com = AG_ComboNew(win, AG_COMBO_ANY_TEXT|AG_COMBO_HFILL, _("Class: "));
	AG_WidgetBind(com->tbox, "string", AG_WIDGET_STRING, ins_tile_class,
	    sizeof(ins_tile_class));
	if (strcmp(ts->template, "Terrain") == 0) {
		AG_TlistAdd(com->list, NULL, "Ground");
		AG_TlistAdd(com->list, NULL, "Rock");
	}

	msb = AG_MSpinbuttonNew(win, 0, "x", _("Size: "));
	AG_WidgetBind(msb, "xvalue", AG_WIDGET_INT, &ins_tile_w);
	AG_WidgetBind(msb, "yvalue", AG_WIDGET_INT, &ins_tile_h);
	AG_MSpinbuttonSetRange(msb, RG_TILE_SIZE_MIN, RG_TILE_SIZE_MAX);

	cb = AG_CheckboxNew(win, 0, _("Alpha blending"));
	AG_WidgetBind(cb, "state", AG_WIDGET_INT, &ins_alpha);
	
	cb = AG_CheckboxNew(win, 0, _("Colorkeying"));
	AG_WidgetBind(cb, "state", AG_WIDGET_INT, &ins_colorkey);
	
	AG_LabelNewStatic(win, _("Snapping mode: "));
	rad = AG_RadioNew(win, AG_RADIO_HFILL, rgTileSnapModes);
	AG_WidgetBind(rad, "value", AG_WIDGET_INT, &ins_snap_mode);

	btnbox = AG_BoxNew(win, AG_BOX_HORIZ, AG_BOX_HFILL|AG_BOX_HOMOGENOUS);
	{
		AG_ButtonAct(btnbox, 0, _("OK"), InsertTile, "%p,%p", win, ts);
		AG_ButtonAct(btnbox, 0, _("Cancel"), AGWINDETACH(win));
	}

	AG_WindowAttach(pwin, win);
	AG_WindowShow(win);
}

static void
InsertTextureDlg(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Window *pwin = AG_PTR(2);
	AG_Window *win;
	AG_Box *btnbox;
	AG_Textbox *tb;

	if ((win = AG_WindowNewNamed(AG_WINDOW_MODAL|AG_WINDOW_NORESIZE|
	    AG_WINDOW_NOMINIMIZE, "rg-instexturedlg")) == NULL) {
		return;
	}
	AG_WindowSetCaption(win, _("Create a new texture"));
	
	tb = AG_TextboxNew(win, AG_TEXTBOX_HFILL|AG_TEXTBOX_FOCUS, _("Name:"));
	AG_WidgetBind(tb, "string", AG_WIDGET_STRING, ins_texture_name,
	    sizeof(ins_texture_name));
	AG_SetEvent(tb, "textbox-return", InsertTexture, "%p,%p,%p", win,
	    pwin, ts);

	btnbox = AG_BoxNew(win, AG_BOX_HORIZ, AG_BOX_HFILL|AG_BOX_HOMOGENOUS);
	{
		AG_ButtonAct(btnbox, 0, _("OK"), InsertTexture,
		    "%p,%p,%p", win, pwin, ts);
		AG_ButtonAct(btnbox, 0, _("Cancel"), AGWINDETACH(win));
	}

	AG_WindowAttach(pwin, win);
	AG_WindowShow(win);
}

static void
InsertAnimDlg(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Window *pwin = AG_PTR(2);
	AG_Window *win;
	AG_Box *btnbox;
	AG_Button *btn;
	AG_Textbox *tb;
	AG_MSpinbutton *msb;
	AG_Checkbox *cb;

	if ((win = AG_WindowNewNamed(AG_WINDOW_MODAL|AG_WINDOW_NORESIZE|
	    AG_WINDOW_NOMINIMIZE, "rg-insanimdlg")) == NULL) {
		return;
	}
	AG_WindowSetCaption(win, _("Create new animation"));
	
	tb = AG_TextboxNew(win, AG_TEXTBOX_HFILL|AG_TEXTBOX_FOCUS, _("Name:"));
	AG_WidgetBind(tb, "string", AG_WIDGET_STRING, ins_anim_name,
	    sizeof(ins_anim_name));
	AG_SetEvent(tb, "textbox-return", InsertAnim, "%p,%p", win, ts);

	msb = AG_MSpinbuttonNew(win, 0, "x", _("Size:"));
	AG_WidgetBind(msb, "xvalue", AG_WIDGET_INT, &ins_tile_w);
	AG_WidgetBind(msb, "yvalue", AG_WIDGET_INT, &ins_tile_h);
	AG_MSpinbuttonSetRange(msb, RG_TILE_SIZE_MIN, RG_TILE_SIZE_MAX);

	cb = AG_CheckboxNew(win, 0, _("Alpha blending"));
	AG_WidgetBind(cb, "state", AG_WIDGET_INT, &ins_alpha);
	
	cb = AG_CheckboxNew(win, 0, _("Colorkey"));
	AG_WidgetBind(cb, "state", AG_WIDGET_INT, &ins_colorkey);

	btnbox = AG_BoxNew(win, AG_BOX_HORIZ, AG_BOX_HFILL|AG_BOX_HOMOGENOUS);
	{
		AG_ButtonAct(btnbox, 0, _("OK"), InsertAnim, "%p,%p", win, ts);
		AG_ButtonAct(btnbox, 0, _("Cancel"), AGWINDETACH(win));
	}

	AG_WindowAttach(pwin, win);
	AG_WindowShow(win);
}

static void
DeleteSelTiles(AG_Event *event)
{
	AG_Tlist *tl = AG_PTR(1);
	RG_Tileset *ts = AG_PTR(2);
	AG_TlistItem *it;

	AG_MutexLock(&ts->lock);
	TAILQ_FOREACH(it, &tl->items, items) {
		RG_Tile *t = it->p1;

		if (!it->selected) {
			continue;
		}
		if (t->nrefs > 0) {
			AG_TextMsg(AG_MSG_ERROR, _("The tile `%s' is in use."),
			    t->name);
			continue;
		}
		TAILQ_REMOVE(&ts->tiles, t, tiles);
		RemoveTileMappings(ts, t);
		RG_TileDestroy(t);
		Free(t, M_RG);
	}
	AG_MutexUnlock(&ts->lock);
}

static void
TileDup(RG_Tileset *ts, RG_Tile *t1)
{
	char name[RG_TILE_NAME_MAX];
	RG_Tile *t2;
	RG_TileElement *e1, *e2;
	int x, y;
	int ncopy = 0;

	t2 = Malloc(sizeof(RG_Tile), M_RG);
tryname1:
	snprintf(name, sizeof(name), _("Copy #%d of %s"), ncopy++, t1->name);
	if (RG_TilesetFindTile(ts, name) != NULL) {
		goto tryname1;
	}
	RG_TileInit(t2, ts, name);
	RG_TileScale(ts, t2, t1->su->w, t1->su->h, t1->flags,
	    t1->su->format->alpha);
	strlcpy(t2->clname, t1->clname, sizeof(t2->clname));

	t2->xOrig = t1->xOrig;
	t2->yOrig = t1->yOrig;
	t2->snap_mode = t1->snap_mode;
	for (y = 0; y < t1->nh; y++) {
		for (x = 0; x < t1->nw; x++) {
			t2->attrs[y*t1->nw+x] = t1->attrs[y*t1->nw+x];
			t2->layers[y*t1->nw+x] = t1->layers[y*t1->nw+x];
		}
	}
	
	TAILQ_FOREACH(e1, &t1->elements, elements) {
		RG_TileElement *e2;
		RG_Pixmap *px1, *px2;

		ncopy = 0;
		switch (e1->type) {
		case RG_TILE_PIXMAP:
			px1 = e1->tel_pixmap.px;
			px2 = Malloc(sizeof(RG_Pixmap), M_RG);
			RG_PixmapInit(px2, ts, 0);
tryname2:
			snprintf(px2->name, sizeof(px2->name),
			    _("Copy #%d of %s"), ncopy++, px1->name);
			if (RG_TilesetFindPixmap(ts, px2->name) != NULL) {
				goto tryname2;
			}
			RG_PixmapScale(px2, px1->su->w, px1->su->h, 0, 0);
			SDL_LockSurface(px1->su);
			SDL_LockSurface(px2->su);
			memcpy((Uint8 *)px2->su->pixels,
			    (Uint8 *)px1->su->pixels,
			    px1->su->h*px1->su->pitch +
			    px1->su->w*px1->su->format->BytesPerPixel);
			SDL_UnlockSurface(px2->su);
			SDL_UnlockSurface(px1->su);

			e2 = RG_TileAddPixmap(t2, e1->name, px2,
			    e1->tel_pixmap.x, e1->tel_pixmap.y);
			e2->visible = e1->visible;
			TAILQ_INSERT_TAIL(&ts->pixmaps, px2, pixmaps);
			break;
		default:
			break;
		}
	}

	RG_TileGenerate(t2);
	TAILQ_INSERT_TAIL(&ts->tiles, t2, tiles);
}

static void
DupSelTiles(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Tlist *tl = AG_PTR(2);
	AG_TlistItem *it;

	AG_MutexLock(&ts->lock);
	TAILQ_FOREACH(it, &tl->items, items) {
		RG_Tile *t = it->p1;

		if (!it->selected) {
			continue;
		}
		TileDup(ts, t);
	}
	AG_MutexUnlock(&ts->lock);
}

static void
EditSelTiles(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Tlist *tl = AG_PTR(2);
	AG_Window *pwin = AG_PTR(3);
	AG_Window *win;
	AG_TlistItem *it;

	AG_MutexLock(&ts->lock);
	TAILQ_FOREACH(it, &tl->items, items) {
		if (!it->selected ||
		    strcmp(it->cat, "tile") != 0) {
			continue;
		}
		if ((win = RG_TileEdit(ts, (RG_Tile *)it->p1)) != NULL) {
			AG_WindowAttach(pwin, win);
			AG_WindowShow(win);
		}
	}
	AG_MutexUnlock(&ts->lock);
}

static void
EditSelAnims(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Tlist *tl = AG_PTR(2);
	AG_Window *pwin = AG_PTR(3);
	AG_Window *win;
	AG_TlistItem *it;

	AG_MutexLock(&ts->lock);
	TAILQ_FOREACH(it, &tl->items, items) {
		if (!it->selected ||
		    strcmp(it->cat, "anim") != 0) {
			continue;
		}
		if ((win = RG_AnimEdit((RG_Anim *)it->p1))
		    != NULL) {
			AG_WindowAttach(pwin, win);
			AG_WindowShow(win);
		}
	}
	AG_MutexUnlock(&ts->lock);
}

static void
EditSelTextures(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Tlist *tl = AG_PTR(2);
	AG_Window *pwin = AG_PTR(3);
	AG_Window *win;
	AG_TlistItem *it;

	AG_MutexLock(&ts->lock);
	TAILQ_FOREACH(it, &tl->items, items) {
		if (!it->selected) {
			continue;
		}
		if ((win = RG_TextureEdit((RG_Texture *)it->p1)) != NULL) {
			AG_WindowAttach(pwin, win);
			AG_WindowShow(win);
		}
	}
	AG_MutexUnlock(&ts->lock);
}

static void
DeleteSelPixmaps(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Tlist *tl_art = AG_PTR(2);
	AG_TlistItem *it;

	if ((it = AG_TlistSelectedItem(tl_art)) != NULL) {
		RG_Pixmap *px = it->p1;
	
		if (px->nrefs > 0) {
			AG_TextMsg(AG_MSG_ERROR,
			    _("The pixmap \"%s\" is currently in use."),
			    px->name);
			return;
		}
		TAILQ_REMOVE(&ts->pixmaps, px, pixmaps);
		RG_PixmapDestroy(px);
		Free(px, M_RG);
	}
}

static void
DeleteSelAnims(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Tlist *tl_anims = AG_PTR(2);
	AG_TlistItem *it;

	if ((it = AG_TlistSelectedItem(tl_anims)) != NULL) {
		RG_Anim *ani = it->p1;
	
		if (ani->nrefs > 0) {
			AG_TextMsg(AG_MSG_ERROR,
			    _("The animation \"%s\" is currently in use."),
			    ani->name);
			return;
		}
		TAILQ_REMOVE(&ts->animations, ani, animations);
		RemoveAnimMappings(ts, ani);
		RG_AnimDestroy(ani);
		Free(ani, M_RG);
	}
}

static void
DeleteSelSketches(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Tlist *tl_art = AG_PTR(2);
	AG_TlistItem *it;

	if ((it = AG_TlistSelectedItem(tl_art)) != NULL) {
		RG_Sketch *sk = it->p1;
	
		if (sk->nrefs > 0) {
			AG_TextMsg(AG_MSG_ERROR,
			    _("The sketch \"%s\" is currently in use."),
			    sk->name);
			return;
		}
		TAILQ_REMOVE(&ts->sketches, sk, sketches);
		RG_SketchDestroy(sk);
		Free(sk, M_RG);
	}
}

static void
DeleteSelTextures(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Tlist *tl_textures = AG_PTR(2);
	AG_TlistItem *it;

	TAILQ_FOREACH(it, &tl_textures->items, items) {
		RG_Texture *tex = it->p1;

		if (!it->selected)
			continue;

		TAILQ_REMOVE(&ts->textures, tex, textures);
		RG_TextureDestroy(tex);
		Free(tex, M_RG);
	}
}

static void
DupSelPixmaps(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);
	AG_Tlist *tl_art = AG_PTR(2);
	AG_TlistItem *it;

	if ((it = AG_TlistSelectedItem(tl_art)) != NULL) {
		RG_Pixmap *px1 = it->p1;
		RG_Pixmap *px2;
		int ncopy = 0;

		px2 = Malloc(sizeof(RG_Pixmap), M_RG);
		RG_PixmapInit(px2, ts, 0);
tryname:
		snprintf(px2->name, sizeof(px2->name), _("Copy #%d of %s"),
		    ncopy++, px1->name);
		if (RG_TilesetFindPixmap(ts, px2->name) != NULL)
			goto tryname;

		RG_PixmapScale(px2, px1->su->w, px1->su->h, 0, 0);
		TAILQ_INSERT_TAIL(&ts->pixmaps, px2, pixmaps);

		SDL_LockSurface(px1->su);
		SDL_LockSurface(px2->su);
		memcpy(
		    (Uint8 *)px2->su->pixels,
		    (Uint8 *)px1->su->pixels,
		    px1->su->h*px1->su->pitch +
		    px1->su->w*px1->su->format->BytesPerPixel);
		SDL_UnlockSurface(px2->su);
		SDL_UnlockSurface(px1->su);
	}
}

static void
SelectTemplate(AG_Event *event)
{
	RG_Tileset *ts = AG_PTR(1);

	RG_TilesetReinit(ts);

	if (strcmp(ts->template, "Perso") == 0) {
		const char *tiles[] = {
			"Idle-N", "Idle-S",
			"Idle-W", "Idle-E"
		};
		const char *anims[] = {
			"Walk-N", "Walk-S",
			"Walk-W", "Walk-E"
		};
		const int ntiles = sizeof(tiles)/sizeof(tiles[0]);
		const int nanims = sizeof(anims)/sizeof(anims[0]);
		RG_Tile *t;
		RG_Anim *a;
		int i;

		for (i = 0; i < ntiles; i++) {
			t = Malloc(sizeof(RG_Tile), M_RG);
			RG_TileInit(t, ts, tiles[i]);
			RG_TileScale(ts, t, 16, 32, RG_TILE_SRCCOLORKEY,
			    SDL_ALPHA_OPAQUE);
			TAILQ_INSERT_TAIL(&ts->tiles, t, tiles);
		}
		for (i = 0; i < nanims; i++) {
			a = Malloc(sizeof(RG_Anim), M_RG);
			RG_AnimInit(a, ts, anims[i], RG_ANIM_SRCCOLORKEY);
			RG_AnimScale(a, 16, 32);
			TAILQ_INSERT_TAIL(&ts->animations, a, animations);
		}
	}
}

static void
PollTileTbl(AG_Event *event)
{
	AG_Tlist *tl = AG_SELF();
	RG_Tileset *ts = AG_PTR(1);
	AG_TlistItem *it;
	Uint32 i;

	AG_TlistClear(tl);
	AG_MutexLock(&ts->lock);
	for (i = 0; i < ts->ntiletbl; i++) {
		if (ts->tiletbl[i] == NULL) {
			it = AG_TlistAdd(tl, NULL, "%u. NULL", (Uint)i);
			it->cat = "";
		} else {
			RG_Tile *t = ts->tiletbl[i];
			
			it = AG_TlistAdd(tl, NULL, "%u. %s (%ux%u)", (Uint)i,
			    t->name, t->su->w, t->su->h);
			it->cat = "tileid";
			it->p1 = t;
			AG_TlistSetIcon(tl, it, t->su);
		}
	}
	AG_TlistRestore(tl);
	AG_MutexUnlock(&ts->lock);
}

static void
PollAnimTbl(AG_Event *event)
{
	AG_Tlist *tl = AG_SELF();
	RG_Tileset *ts = AG_PTR(1);
	AG_TlistItem *it;
	Uint32 i;

	AG_TlistClear(tl);
	AG_MutexLock(&ts->lock);
	for (i = 0; i < ts->nanimtbl; i++) {
		if (ts->animtbl[i] == NULL) {
			it = AG_TlistAdd(tl, NULL, "%u. NULL", (Uint)i);
			it->cat = "";
		} else {
			RG_Anim *anim = ts->animtbl[i];
			
			it = AG_TlistAdd(tl, NULL, "%u. %s (%ux%u)", (Uint)i,
			    anim->name, anim->w, anim->h);
			it->cat = "animid";
			it->p1 = anim;
#if 0
			if (anim->nframes > 0)
				AG_TlistSetIcon(tl, it, anim->frames[0].su)
#endif
		}
	}
	AG_TlistRestore(tl);
	AG_MutexUnlock(&ts->lock);
}

void *
RG_TilesetEdit(void *p)
{
	RG_Tileset *ts = p;
	AG_Window *win;
	AG_Tlist *tl_tiles, *tl_art, *tl_anims, *tl_textures;
	AG_Tlist *tl_tiletbl, *tl_animtbl;
	AG_Box *box, *hbox, *bbox;
	AG_Textbox *tb;
	AG_MSpinbutton *msb;
	AG_Menu *m;
	AG_MenuItem *mi;
	AG_Button *bu;
	AG_Notebook *nb;
	AG_NotebookTab *ntab;
	AG_Combo *com;

	win = AG_WindowNew(0);
	AG_WindowSetCaption(win, _("Tile set: %s"), AGOBJECT(ts)->name);
	AG_WindowSetPosition(win, AG_WINDOW_LOWER_CENTER, 1);

	tl_tiles = Malloc(sizeof(AG_Tlist), M_OBJECT);
	AG_TlistInit(tl_tiles, AG_TLIST_POLL|AG_TLIST_MULTI|AG_TLIST_TREE|
		               AG_TLIST_EXPAND);
	AG_TlistPrescale(tl_tiles, "XXXXXXXXXXXXXXXXXXXXXXXX (00x00)", 6);
	AG_SetEvent(tl_tiles, "tlist-poll", PollTiles, "%p", ts);
	
	tl_art = Malloc(sizeof(AG_Tlist), M_OBJECT);
	AG_TlistInit(tl_art, AG_TLIST_POLL|AG_TLIST_EXPAND);
	AG_SetEvent(tl_art, "tlist-poll", PollGraphics, "%p", ts);
	
	tl_textures = Malloc(sizeof(AG_Tlist), M_OBJECT);
	AG_TlistInit(tl_textures, AG_TLIST_POLL|AG_TLIST_EXPAND);
	AG_SetEvent(tl_textures, "tlist-poll", PollTextures, "%p", ts);

	tl_anims = Malloc(sizeof(AG_Tlist), M_OBJECT);
	AG_TlistInit(tl_anims, AG_TLIST_POLL|AG_TLIST_EXPAND);
	AG_SetEvent(tl_anims, "tlist-poll", PollAnims, "%p", ts);
	
	tl_tiletbl = Malloc(sizeof(AG_Tlist), M_OBJECT);
	AG_TlistInit(tl_tiletbl, AG_TLIST_POLL|AG_TLIST_EXPAND);
	AG_SetEvent(tl_tiletbl, "tlist-poll", PollTileTbl, "%p", ts);
	
	tl_animtbl = Malloc(sizeof(AG_Tlist), M_OBJECT);
	AG_TlistInit(tl_animtbl, AG_TLIST_POLL|AG_TLIST_EXPAND);
	AG_SetEvent(tl_animtbl, "tlist-poll", PollAnimTbl, "%p", ts);

	com = AG_ComboNew(win, AG_COMBO_HFILL, _("Template: "));
	AG_WidgetBind(com->tbox, "string", AG_WIDGET_STRING, &ts->template,
	    sizeof(ts->template));
	AG_TlistAdd(com->list, NULL, "Perso");
	AG_TlistAdd(com->list, NULL, "Terrain");
	AG_SetEvent(com, "combo-selected", SelectTemplate, "%p", ts);

	mi = AG_TlistSetPopup(tl_tiles, "tile");
	{
		AG_MenuAction(mi, _("Edit tiles..."), OBJEDIT_ICON,
		    EditSelTiles, "%p,%p,%p", ts, tl_tiles, win);
		AG_MenuSeparator(mi);
		AG_MenuAction(mi, _("Duplicate tiles"),
		    OBJDUP_ICON,
		    DupSelTiles, "%p,%p", ts, tl_tiles);
		AG_MenuSeparator(mi);
		AG_MenuAction(mi, _("Delete tiles"), TRASH_ICON,
		    DeleteSelTiles, "%p,%p", tl_tiles, ts);
	}

	nb = AG_NotebookNew(win, AG_NOTEBOOK_HFILL|AG_NOTEBOOK_VFILL);
	ntab = AG_NotebookAddTab(nb, _("Tiles"), AG_BOX_VERT);
	{
		AG_ObjectAttach(ntab, tl_tiles);
		AG_SetEvent(tl_tiles, "tlist-dblclick", EditSelTiles,
		    "%p,%p,%p", ts, tl_tiles, win);

		bbox = AG_BoxNew(ntab, AG_BOX_HORIZ,
		    AG_BOX_HFILL|AG_BOX_HOMOGENOUS);
		{
			AG_ButtonAct(bbox, 0, _("Insert"), InsertTileDlg,
			    "%p,%p", ts, win);
			AG_ButtonAct(bbox, 0, _("Edit"), EditSelTiles,
			    "%p,%p,%p", ts, tl_tiles, win);
			AG_ButtonAct(bbox, 0, _("Delete"), DeleteSelTiles,
			    "%p,%p", tl_tiles, ts);
		}
	}

	ntab = AG_NotebookAddTab(nb, _("Graphics"), AG_BOX_VERT);
	{
		AG_ObjectAttach(ntab, tl_art);
	
		mi = AG_TlistSetPopup(tl_art, "pixmap");
		{
			AG_MenuAction(mi, _("Delete pixmap"), TRASH_ICON,
			    DeleteSelPixmaps, "%p,%p", ts, tl_art);
			AG_MenuAction(mi, _("Duplicate pixmap"), OBJDUP_ICON,
			    DupSelPixmaps, "%p,%p", ts, tl_art);
		}
		mi = AG_TlistSetPopup(tl_art, "sketch");
		{
			AG_MenuAction(mi, _("Delete sketch"), TRASH_ICON,
			    DeleteSelSketches, "%p,%p", ts, tl_art);
		}
	}
	
	ntab = AG_NotebookAddTab(nb, _("Textures"), AG_BOX_VERT);
	{
		AG_ObjectAttach(ntab, tl_textures);
	
		mi = AG_TlistSetPopup(tl_textures, "texture");
		{
			AG_MenuAction(mi, _("Delete texture"), TRASH_ICON,
			    DeleteSelTextures, "%p,%p", ts, tl_textures);
		}
		
		bbox = AG_BoxNew(ntab, AG_BOX_HORIZ, 
		    AG_BOX_HFILL|AG_BOX_HOMOGENOUS);
		{
			AG_ButtonAct(bbox, 0, _("Insert"),
			    InsertTextureDlg, "%p,%p", ts, win);
			AG_ButtonAct(bbox, 0, _("Edit"),
			    EditSelTextures, "%p,%p,%p", ts, tl_textures, win);
			AG_ButtonAct(bbox, 0, _("Delete"),
			    DeleteSelTextures, "%p,%p", ts, tl_textures);
		}
		
		AG_SetEvent(tl_textures, "tlist-dblclick", EditSelTextures,
		    "%p,%p,%p", ts, tl_textures, win);
	}
	
	ntab = AG_NotebookAddTab(nb, _("Animations"), AG_BOX_VERT);
	{
		AG_ObjectAttach(ntab, tl_anims);
		AG_SetEvent(tl_anims, "tlist-dblclick",
		    EditSelAnims, "%p,%p,%p", ts, tl_anims, win);
		
		mi = AG_TlistSetPopup(tl_anims, "anim");
		{
			AG_MenuAction(mi, _("Edit animation"), EDIT_ICON,
			    EditSelAnims, "%p,%p,%p", ts, tl_anims, win);
			AG_MenuAction(mi, _("Delete animation"), TRASH_ICON,
			    DeleteSelAnims, "%p,%p", ts, tl_anims);
		}
		
		bbox = AG_BoxNew(ntab, AG_BOX_HORIZ,
		    AG_BOX_HFILL|AG_BOX_HOMOGENOUS);
		{
			AG_ButtonAct(bbox, 0, _("Insert"),
			    InsertAnimDlg, "%p,%p", ts, win);
			AG_ButtonAct(bbox, 0, _("Edit"),
			    EditSelAnims, "%p,%p,%p", ts, tl_anims, win);
			AG_ButtonAct(bbox, 0, _("Delete"),
			    DeleteSelAnims, "%p,%p", ts, tl_anims);
		}
	}

	ntab = AG_NotebookAddTab(nb, _("Tile IDs"), AG_BOX_VERT);
	{
		AG_ObjectAttach(ntab, tl_tiletbl);
#if 0
		mi = AG_TlistSetPopup(tl_tiletbl, "tileid");
		AG_MenuAction(mi, _("Invalidate mapping"), TRASH_ICON,
		    InvalidateTileIDs, "%p,%p", ts, tl_tiletbl);
		bbox = AG_BoxNew(ntab, AG_BOX_HORIZ,
		    AG_BOX_HFILL|AG_BOX_HOMOGENOUS);
		AG_ButtonAct(bbox, 0, _("Invalidate"),
		    InvalidateTileIDs, "%p,%p", ts, tl_tiletbl);
#endif
	}
	
	ntab = AG_NotebookAddTab(nb, _("Anim IDs"), AG_BOX_VERT);
	{
		AG_ObjectAttach(ntab, tl_animtbl);
#if 0
		mi = AG_TlistSetPopup(tl_animtbl, "animid");
		AG_MenuAction(mi, _("Invalidate mapping"), TRASH_ICON,
		    InvalidateAnimIDs, "%p,%p", ts, tl_animtbl);
		bbox = AG_BoxNew(ntab, AG_BOX_HORIZ,
		    AG_BOX_HFILL|AG_BOX_HOMOGENOUS);
		AG_ButtonAct(bbox, 0, _("Invalidate"),
		    InvalidateAnimIDs, "%p,%p", ts, tl_animtbl);
#endif
	}
	return (win);
}

#endif /* EDITION */
