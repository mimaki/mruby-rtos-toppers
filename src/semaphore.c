#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/hash.h"
#include "rtosif.h"

/*
 *  call-seq:
 *     Semaphore.new(id=nil, max=1, init=1, *modes) # => Semaphore
 *
 *  Creates Semaphore object.
 *
 *  Parameters:
 *    +id+      Event flag ID
 *      nil       Assign ID automatically.
 *		+max+			Maximum count of semaphore (defalut=1)
 *		+init+		Initial count of semaphore (default=1)
 *    +*modes+  Event flag attribute
 *      :fifo       task waiting FIFO queue (default)
 *      :priority   task Waiting priority queue
 *
 *  Returns Semaphore object.
 */
static mrb_value
mrb_sem_init(mrb_state *mrb, mrb_value self)
{
  struct RClass *flg = mrb_obj_class(mrb, self);
  mrb_value amap = mrb_const_get(mrb, mrb_obj_value(flg), mrb_intern_lit(mrb, "ATTRIBUTE"));
  mrb_value ido = mrb_nil_value();
	mrb_int max = 1;
	mrb_int init = 1;
  mrb_value *pmode;
  mrb_int len;
  T_CSEM csem = {0, 1, 1};
  mrb_int id;
  mrb_int i;
  ATR sematr = 0;
  mrb_value md;

  mrb_get_args(mrb, "|oii*", &ido, &max, &init, &pmode, &len);

  for (i=0; i<len; i++) {
    md = mrb_hash_get(mrb, amap, pmode[i]);
    if (mrb_nil_p(md)) {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid semaphore attribute :%S", mrb_sym2str(mrb, mrb_symbol(pmode[i])));
    }
    sematr |= mrb_fixnum(md);
  }
  csem.sematr = sematr;
	csem.isemcnt = init;
	csem.maxsem = max;

  if (mrb_nil_p(ido)) {
    id = RTOS_acre_sem(mrb, &csem);
  }
  else {
    id = mrb_fixnum(ido);
    // TODO: cre_sem(id, csem);
  }
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@id"), mrb_fixnum_value(id));

  return self;
}

/*
 *  call-seq:
 *     sem.release	# => nil
 *
 *  Release semaphore.
 *
 *  Parameters:
 *    none
 *
 *  Returns nil.
 */
static mrb_value
mrb_sem_sig(mrb_state *mrb, mrb_value self)
{
  mrb_int id = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id")));

  RTOS_sig_sem(mrb, (ID)id);

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     sem.wait(tmout=:forever)	# => true / false
 *
 *  Wait semaphore.
 *
 *  Parameters:
 *    +tmout+
 *      :forever  Wait forever
 *      :polling  No wait
 *
 *  Returns EventFlag signaled value.
 */
static mrb_value
mrb_sem_wait(mrb_state *mrb, mrb_value self)
{
  mrb_value tmap = mrb_mod_cv_get(mrb, mrb_module_get(mrb, "RTOS"), mrb_intern_lit(mrb, "TIMEOUT"));
  mrb_int id = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id")));
  mrb_value tmo = mrb_symbol_value(mrb_intern_lit(mrb, "forever"));
  mrb_value tmov;
	int er = 0;

  mrb_get_args(mrb, "|o", &tmo);

  if (mrb_fixnum_p(tmo)) {
    er = RTOS_twai_sem(mrb, id, mrb_fixnum(tmo));
  }
  else if (mrb_symbol_p(tmo)) {
    tmov = mrb_hash_get(mrb, tmap, tmo);
    if (mrb_nil_p(tmov)) {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid timeout value :%S", mrb_sym2str(mrb, mrb_symbol(tmo)));
    }
    if (mrb_symbol(tmo) == mrb_intern_lit(mrb, "forever")) {
      er = RTOS_wai_sem(mrb, id);
    }
    else {  /* polling */
      er = RTOS_pol_sem(mrb, id);
    }
  }
  else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,  "invalid timeout value :%S", mrb_funcall(mrb, tmo, "to_s", 0));
  }

  return mrb_bool_value(er == E_OK);
}


void
mrb_rtos_sem_init(mrb_state *mrb, struct RClass *rtos)
{
  struct RClass *sem;
  mrb_value semo;
  mrb_value atr;

  /* Semaphore class */
  sem = mrb_define_class_under(mrb, rtos, "Semaphore", mrb->object_class);
  semo = mrb_obj_value(sem);

  atr = mrb_hash_new(mrb);
  mrb_hash_set(mrb, atr, mrb_symbol_value(mrb_intern_lit(mrb, "fifo")),     mrb_fixnum_value(TA_TFIFO));
  mrb_hash_set(mrb, atr, mrb_symbol_value(mrb_intern_lit(mrb, "priority")), mrb_fixnum_value(TA_TPRI));
  mrb_const_set(mrb, semo, mrb_intern_lit(mrb, "ATTRIBUTE"), atr);

  mrb_define_method(mrb, sem, "initialize", mrb_sem_init,      MRB_ARGS_ANY());
  mrb_define_method(mrb, sem, "release",   	mrb_sem_sig,       MRB_ARGS_NONE());
  mrb_define_method(mrb, sem, "wait",       mrb_sem_wait,      MRB_ARGS_OPT(1));
}
