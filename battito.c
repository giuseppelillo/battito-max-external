/**
	@file
	battito - a max object

	@ingroup	examples
*/

#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object
#include "battito_max.h"

#define DEFAULT_SUBDIVISION 1920

////////////////////////// object struct
typedef struct _battito
{
	t_object ob;
    long subdivision;
    struct event* events;
    long length;
    void *m_outlet1;
    void *m_outlet2;
    void *m_outlet3;
} t_battito;

///////////////////////// function prototypes
//// standard set
void *battito_new(t_symbol *s, long argc, t_atom *argv);
void battito_free(t_battito *x);
void battito_int(t_battito *x, long n);
void battito_msg(t_battito *x, t_symbol *s);
void battito_reset(t_battito *x, t_symbol *s);

//////////////////////// global class pointer variable
void *battito_class;

void ext_main(void *r)
{
	t_class *c;

	c = class_new("battito", (method)battito_new, (method)battito_free, (long)sizeof(t_battito), 0L, A_GIMME, 0);

    class_addmethod(c, (method)battito_int, "int", A_LONG, 0);
    class_addmethod(c, (method)battito_msg, "text", A_SYM, 0);
    class_addmethod(c, (method)battito_reset, "reset", A_SYM, 0);
    
	class_register(CLASS_BOX, c);
	battito_class = c;
}

void battito_int(t_battito *x, long n)
{
    if (x->length > 0) {
        long index = labs(n) % x->length;
//        post("index: %ld", index);
        struct event event = x->events[index];
        uint32_t value = event.value;
        uint32_t probability = event.probability;
    //    post("%d : %d", value, probability);
        if (probability > 0) {
            outlet_int(x->m_outlet2, probability);
            outlet_int(x->m_outlet3, value);
        }
    }
}

void battito_msg(t_battito *x, t_symbol *s)
{
    if (x->events != NULL) {
//        object_post((t_object *)x, "free: %p", x->events);
        free(x->events);
        x->events = NULL;
    }
    struct pattern pattern = transform(s->s_name, x->subdivision);
    x->events = pattern.events;
    x->length = pattern.length * x->subdivision;
    outlet_int(x->m_outlet1, x->length);
}

void battito_reset(t_battito *x, t_symbol *s)
{
    x->events = NULL;
    x->length = 0;
}

void battito_free(t_battito *x)
{
	;
}


void *battito_new(t_symbol *s, long argc, t_atom *argv)
{
	t_battito *x = NULL;

	if ((x = (t_battito *)object_alloc(battito_class))) {
        if (argc == 1 && (argv)->a_type == A_LONG) {
            x->subdivision = atom_getlong(argv);
        } else if (argc == 0){
            x->subdivision = DEFAULT_SUBDIVISION;
        } else {
            object_error((t_object *)x, "Wrong arguments");
            x->subdivision = DEFAULT_SUBDIVISION;
        }

        x->events = NULL;
        x->length = 0;
        x->m_outlet1 = intout((t_object *)x);
        x->m_outlet2 = intout((t_object *)x);
        x->m_outlet3 = intout((t_object *)x);
        
	}
	return (x);
}
