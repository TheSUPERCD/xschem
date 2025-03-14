/* File: hash_iterator.c
 *
 * This file is part of XSCHEM,
 * a schematic capture and Spice/Vhdl/Verilog netlisting tool for circuit
 * simulation.
 * Copyright (C) 1998-2024 Stefan Frederik Schippers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "xschem.h"

void init_inst_iterator(Iterator_ctx *ctx, double x1, double y1, double x2, double y2)
{
      ctx->instflag = NULL;
      dbg(3, "init_inst_iterator(): instances=%d\n", xctx->instances);
      
      if(xctx->instances) {
        my_realloc(_ALLOC_ID_, &ctx->instflag, xctx->instances*sizeof(unsigned short));
        memset(ctx->instflag, 0, xctx->instances*sizeof(unsigned short));
      }
      /* calculate square 4 1st corner of drawing area */
      ctx->x1a = (int)floor(x1 / BOXSIZE) ;
      ctx->y1a = (int)floor(y1 / BOXSIZE) ;
      /* calculate square 4 2nd corner of drawing area */
      ctx->x2a = (int)floor(x2 / BOXSIZE);
      ctx->y2a = (int)floor(y2 / BOXSIZE);
      ctx->i = ctx->x1a;
      ctx->j = ctx->y1a;
      ctx->tmpi = ctx->i % NBOXES; if(ctx->tmpi < 0) ctx->tmpi += NBOXES;
      ctx->tmpj = ctx->j % NBOXES; if(ctx->tmpj < 0) ctx->tmpj += NBOXES;
      ctx->counti = 0;
      ctx->instanceptr = xctx->inst_spatial_table[ctx->tmpi][ctx->tmpj];
      ctx->countj = 0;
}


Instentry *inst_iterator_next(Iterator_ctx *ctx)
{
  Instentry *ptr;
  /* dbg(3, "inst_iterator_next(): instances=%d\n", xctx->instances); */
  while(1) {
    while(ctx->instanceptr) {
      ptr = ctx->instanceptr;
      ctx->instanceptr = ctx->instanceptr->next;
      if(!ctx->instflag[ptr->n]) {
        ctx->instflag[ptr->n]=1;
        return ptr;
      }
    }
    if(ctx->j < ctx->y2a && ctx->countj++ < NBOXES) {
      ctx->j++;
      ctx->tmpj = ctx->j % NBOXES; if(ctx->tmpj < 0) ctx->tmpj+=NBOXES;
      ctx->instanceptr = xctx->inst_spatial_table[ctx->tmpi][ctx->tmpj];
    } else if(ctx->i < ctx->x2a && ctx->counti++ < NBOXES) {
      ctx->i++;
      ctx->j = ctx->y1a;
      ctx->countj = 0;
      ctx->tmpi = ctx->i % NBOXES; if(ctx->tmpi < 0) ctx->tmpi += NBOXES;
      ctx->tmpj = ctx->j % NBOXES; if(ctx->tmpj < 0) ctx->tmpj += NBOXES;
      ctx->instanceptr = xctx->inst_spatial_table[ctx->tmpi][ctx->tmpj];
    } else {
      my_free(_ALLOC_ID_, &ctx->instflag);
      return NULL;
    }
  }
}

void init_wire_iterator(Iterator_ctx *ctx, double x1, double y1, double x2, double y2)
{
      ctx->wireflag = NULL;
      dbg(3, "init_wire_iterator(): wires=%d\n", xctx->wires);
      if(xctx->wires) {
        my_realloc(_ALLOC_ID_, &ctx->wireflag, xctx->wires*sizeof(unsigned short));
        memset(ctx->wireflag, 0, xctx->wires*sizeof(unsigned short));
      }
      /* calculate square 4 1st corner of drawing area */
      ctx->x1a = (int)floor(x1 / BOXSIZE) ;
      ctx->y1a = (int)floor(y1 / BOXSIZE) ;
      /* calculate square 4 2nd corner of drawing area */
      ctx->x2a = (int)floor(x2 / BOXSIZE);
      ctx->y2a = (int)floor(y2 / BOXSIZE);
      ctx->i = ctx->x1a;
      ctx->j = ctx->y1a;
      ctx->tmpi = ctx->i % NBOXES; if(ctx->tmpi < 0) ctx->tmpi += NBOXES;
      ctx->tmpj = ctx->j % NBOXES; if(ctx->tmpj < 0) ctx->tmpj += NBOXES;
      ctx->counti = 0;
      ctx->wireptr = xctx->wire_spatial_table[ctx->tmpi][ctx->tmpj];
      ctx->countj = 0;
}


Wireentry *wire_iterator_next(Iterator_ctx *ctx)
{
  Wireentry *ptr;
  /* dbg(3, "wire_iterator_next(): wires=%d\n", xctx->wires); */
  while(1) {
    while(ctx->wireptr) {
      ptr = ctx->wireptr;
      ctx->wireptr = ctx->wireptr -> next;
      if(!ctx->wireflag[ptr->n]) {
        ctx->wireflag[ptr->n]=1;
        return ptr;
      }
    }
    if(ctx->j < ctx->y2a && ctx->countj++ < NBOXES) {
      ctx->j++;
      ctx->tmpj = ctx->j % NBOXES; if(ctx->tmpj < 0) ctx->tmpj += NBOXES;
      ctx->wireptr = xctx->wire_spatial_table[ctx->tmpi][ctx->tmpj];
    } else if(ctx->i < ctx->x2a && ctx->counti++ < NBOXES) {
      ctx->i++;
      ctx->j = ctx->y1a;
      ctx->countj = 0;
      ctx->tmpi = ctx->i % NBOXES; if(ctx->tmpi < 0) ctx->tmpi += NBOXES;
      ctx->tmpj = ctx->j % NBOXES; if(ctx->tmpj < 0) ctx->tmpj += NBOXES;
      ctx->wireptr = xctx->wire_spatial_table[ctx->tmpi][ctx->tmpj];
    } else {
      my_free(_ALLOC_ID_, &ctx->wireflag);
      return NULL;
    }
  }
}


void init_object_iterator(Iterator_ctx *ctx, double x1, double y1, double x2, double y2)
{
      ctx->objectflag = NULL;
      dbg(3, "init_object_iterator(): objects=%d\n", xctx->n_hash_objects);
      if(xctx->n_hash_objects) {
        my_realloc(_ALLOC_ID_, &ctx->objectflag, xctx->n_hash_objects * sizeof(unsigned short));
        memset(ctx->objectflag, 0, xctx->n_hash_objects * sizeof(unsigned short));
      }
      /* calculate square 4 1st corner of drawing area */
      ctx->x1a = (int)floor(x1 / BOXSIZE) ;
      ctx->y1a = (int)floor(y1 / BOXSIZE) ;
      /* calculate square 4 2nd corner of drawing area */
      ctx->x2a = (int)floor(x2 / BOXSIZE);
      ctx->y2a = (int)floor(y2 / BOXSIZE);
      ctx->i = ctx->x1a;
      ctx->j = ctx->y1a;
      ctx->tmpi = ctx->i % NBOXES; if(ctx->tmpi < 0) ctx->tmpi += NBOXES;
      ctx->tmpj = ctx->j % NBOXES; if(ctx->tmpj < 0) ctx->tmpj += NBOXES;
      ctx->counti = 0;
      ctx->objectptr = xctx->object_spatial_table[ctx->tmpi][ctx->tmpj];
      ctx->countj = 0;
}

Objectentry *object_iterator_next(Iterator_ctx *ctx)
{
  Objectentry *ptr;
  /* dbg(3, "object_iterator_next(): obhjects=%d\n", xctx->n_hash_objects); */
  while(1) {
    while(ctx->objectptr) {
      ptr = ctx->objectptr;
      ctx->objectptr = ctx->objectptr->next;
      if(!ctx->objectflag[ptr->n]) {
        ctx->objectflag[ptr->n] = 1;
        return ptr;
      }
    }
    if(ctx->j < ctx->y2a && ctx->countj++ < NBOXES) {
      ctx->j++;
      ctx->tmpj = ctx->j % NBOXES; if(ctx->tmpj < 0) ctx->tmpj += NBOXES;
      ctx->objectptr = xctx->object_spatial_table[ctx->tmpi][ctx->tmpj];
    } else if(ctx->i < ctx->x2a && ctx->counti++ < NBOXES) {
      ctx->i++;
      ctx->j = ctx->y1a;
      ctx->countj = 0;
      ctx->tmpi = ctx->i % NBOXES; if(ctx->tmpi < 0) ctx->tmpi += NBOXES;
      ctx->tmpj = ctx->j % NBOXES; if(ctx->tmpj < 0) ctx->tmpj += NBOXES;
      ctx->objectptr = xctx->object_spatial_table[ctx->tmpi][ctx->tmpj];
    } else {
      my_free(_ALLOC_ID_, &ctx->objectflag);
      return NULL;
    }
  }
}


