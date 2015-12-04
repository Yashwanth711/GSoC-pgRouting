/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/
/*
This test is for the equivalence pgr_trsp when:
- no restrictions are given 

With pgr_dijkstra one To One signature
for both: directed and undirected versions

*/

\set ECHO none
\set QUIET 1
-- Turn off echo and keep things quiet.

-- Format the output for nice TAP.
\pset format unaligned
\pset tuples_only true
\pset pager
\set VERBOSITY terse

-- Revert all changes on failure.
\set ON_ERROR_ROLLBACK true
\set ON_ERROR_STOP true
\set QUIET 1

BEGIN;
    SELECT plan(1296);

    UPDATE edge_table SET cost = cost + 0.001 * id, reverse_cost = reverse_cost + 0.001 * id;
    SELECT * FROM edge_table;


    create or REPLACE FUNCTION foo(cant INTEGER default 18, flag boolean default true )
    RETURNS SETOF TEXT AS
    $BODY$
    DECLARE
    dijkstra_sql TEXT;
    trsp_sql TEXT;
    inner_sql1 TEXT;
    inner_sql2 TEXT;
    k integer;
    directed TEXT;
    msg TEXT;
    BEGIN
        directed = 'Undirected';
        IF flag THEN directed = 'Directed'; END IF;
        k := 1;
        inner_sql1 = quote_literal('SELECT id, source, target, cost, reverse_cost from edge_table ORDER BY id');
        inner_sql2 = quote_literal('SELECT id, source, target, cost from edge_table ORDER BY id');
        FOR i IN 1.. cant LOOP
            FOR j IN 1..cant LOOP
                CONTINUE WHEN i = j;
                dijkstra_sql := 'SELECT seq-1, node::integer, edge::integer, cost
                    FROM pgr_dijkstra( ' || inner_sql1 || ', ' || i || ', ' || j || ', ' || flag || ')';
                RAISE NOTICE '%', dijkstra_sql;
                trsp_sql := 'SELECT * from pgr_trsp( ' || inner_sql1 || ', ' || i || ', ' || j || ', ' || flag || ', true)';
                RAISE NOTICE '%', trsp_sql;
                msg := k || ' ' || directed || ', with reverse_cost: from '  || i || ' to ' || j;
                BEGIN
                    EXECUTE trsp_sql;
                    RETURN query SELECT set_eq(trsp_sql, dijkstra_sql, msg);
                    EXCEPTION WHEN OTHERS THEN
                        msg := k || ' ' || directed || ', with reverse_cost: from '  || i || ' to ' || j || '... Exception found ' ;
                        RETURN query SELECT is_empty(dijkstra_sql, msg);
                END;
                k := k + 1;

                /*
                dijkstra_sql := 'SELECT seq-1, node::integer, edge::integer, cost
                    FROM pgr_dijkstra( ' || inner_sql2 || ', ' || i || ', ' || j || ', ' || flag || ')';
                trsp_sql := 'SELECT * from pgr_trsp( ' || inner_sql2 || ', ' || i || ', ' || j || ', ' || flag || ', false)';
                msg := k || ' ' || directed || ', no reverse_cost: from '  || i || ' to ' || j;
                RETURN query SELECT set_eq(dijkstra_sql, trsp_sql, msg);
                k := k + 1;
*/
            END LOOP;
        END LOOP;
    END
    $BODY$
    language plpgsql;

SELECT * from foo(18, true);
--SELECT * from foo(3, false);


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

