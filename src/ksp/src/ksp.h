/*
 * KShortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2012 Dave Potts
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */
 
#ifndef _KSP_H
#define _KSP_H

#define TUPLIMIT 1000
#define PATH_ALLOC_TOTAL 5

#include <unistd.h>
#include "postgres.h"

#undef DEBUG
//#define DEBUG

#ifdef __cplusplus
extern "C"
{
#endif


#include "./../../common/src/pgr_types.h"

#if 0
typedef struct ksp_edge 
{
    int64_t id;
    int64_t source;
    int64_t target;
    float8 cost;
    float8 reverse_cost;
} ksp_edge_t;

typedef struct ksp_path_element 
{
    int route_id;
    int64_t vertex_id;
    int64_t edge_id;
    float8 cost;
} ksp_path_element_t;

typedef struct ksp_edge_columns 
{
  int64_t id;
  int64_t source;
  int64_t target;
  float8  cost;
  float8  reverse_cost;
} ksp_edge_columns_t;
#endif

int compute_kshortest_path(char* sql, int64_t start_vertex, 
                                 int64_t end_vertex, int no_paths, 
                                 bool has_reverse_cost, bool directedFlag,
                                 ksp_path_element_t **path, int *ksp_path_count) ;

ksp_path_element_t * get_ksp_memory(int, ksp_path_element_t *path);


//extern void kspDBG(const char* format, ...);
#ifdef __cplusplus
}
#endif

#endif // _KSP_H
