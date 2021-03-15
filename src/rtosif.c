#include "ev3api.h"
#include "mruby.h"
#include "mruby/variable.h"

/* Macros */
#define RTOS_VERIFY(mrb, er) {  \
    if(er < E_OK) {             \
      mrb_raisef(mrb, E_RUNTIME_ERROR, "%S error (%S)", mrb_str_new_cstr(mrb, __FUNCTION__), mrb_fixnum_value(er)); \
    }                           \
  }
#define RTOS_VERIFY_TIMEOUT(mrb, er) {  \
    if(er < E_OK && er != E_TMOUT) {    \
      mrb_raisef(mrb, E_RUNTIME_ERROR, "%S error (%S)", mrb_str_new_cstr(mrb, __FUNCTION__), mrb_fixnum_value(er)); \
    }                                   \
  }

/* Task */
ER
RTOS_tslp_tsk(mrb_state *mrb, TMO tmo)
{
  ER er = tslp_tsk(tmo);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return er;
}

ER
RTOS_get_tim(mrb_state *mrb, SYSTIM *st)
{
  ER er = get_tim(st);
  RTOS_VERIFY(mrb, er);
  return er;
}

ER
RTOS_act_tsk(mrb_state *mrb, ID id)
{
  ER er = act_tsk(id);
  RTOS_VERIFY(mrb, er);
  return er;
}

ER
RTOS_sus_tsk(mrb_state *mrb, ID id)
{
  ER er = sus_tsk(id);
  RTOS_VERIFY(mrb, er);
  return er;
}

ER
RTOS_rsm_tsk(mrb_state *mrb, ID id)
{
  ER er = rsm_tsk(id);
  RTOS_VERIFY(mrb, er);
  return er;
}

ER
RTOS_ter_tsk(mrb_state *mrb, ID id)
{
  ER er = ter_tsk(id);
  RTOS_VERIFY(mrb, er);
  return er;
}


/* Semaphore */
ER_ID RTOS_acre_sem(mrb_state *mrb, T_CSEM *pcsem)
{
  ID id = acre_sem(pcsem);
  RTOS_VERIFY(mrb, id);
  return id;
}

ER RTOS_sig_sem(mrb_state *mrb, ID id)
{
  ER er = sig_sem(id);
  RTOS_VERIFY(mrb, er);
  return er;
}

ER RTOS_wai_sem(mrb_state *mrb, ID id)
{
  ER er = wai_sem(id);
  RTOS_VERIFY(mrb, er);
  return er;
}

ER RTOS_pol_sem(mrb_state *mrb, ID id)
{
  ER er = pol_sem(id);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return er;
}

ER RTOS_twai_sem(mrb_state *mrb, ID id, TMO tmout)
{
  ER er = twai_sem(id, tmout);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return er;
}


/* Event flag */
ER_ID
RTOS_acre_flg(mrb_state *mrb, T_CFLG *pk_cflg)
{
  ID id = acre_flg(pk_cflg);
  RTOS_VERIFY(mrb, id);
  return id;
}

ER
RTOS_set_flg(mrb_state *mrb, ID id, FLGPTN setptn)
{
  ER er = set_flg(id, setptn);
  RTOS_VERIFY(mrb, er);
  return er;
}

ER
RTOS_wai_flg(mrb_state *mrb, ID id, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
  ER er = wai_flg(id, waiptn, wfmode, p_flgptn);
  RTOS_VERIFY(mrb, er);
  return er;
}

ER
RTOS_pol_flg(mrb_state *mrb, ID id, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
  ER er = pol_flg(id, waiptn, wfmode, p_flgptn);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return er;
}

ER
RTOS_twai_flg(mrb_state *mrb, ID id, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn, TMO tmout)
{
  ER er = twai_flg(id, waiptn, wfmode, p_flgptn, tmout);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return er;
}

ER
RTOS_clr_flg(mrb_state *mrb, ID id, FLGPTN clrptn)
{
  ER er = clr_flg(id, clrptn);
  RTOS_VERIFY(mrb, er);
  return er;
}


/* Fixed size memory pool */
ER_ID
RTOS_acre_mpf(mrb_state *mrb, T_CMPF *pcmpf)
{
  ER er = acre_mpf(pcmpf);
  RTOS_VERIFY(mrb, er);
  return er;
}

ER RTOS_get_mpf(mrb_state *mrb, ID id, void **pblk)
{
  ER er = get_mpf(id, pblk);
  RTOS_VERIFY(mrb, er);
  return er;
}

ER RTOS_pget_mpf(mrb_state *mrb, ID id, void **pblk)
{
  ER er = pget_mpf(id, pblk);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return er;
}

ER RTOS_tget_mpf(mrb_state *mrb, ID id, void **pblk, TMO tmo)
{
  ER er = tget_mpf(id, pblk, tmo);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return er;
}

ER RTOS_rel_mpf(mrb_state *mrb, ID id, void *pblk)
{
  ER er = rel_mpf(id, pblk);
  RTOS_VERIFY(mrb, er);
  return er;
}


/* Data Queue */
ER_ID	RTOS_acre_dtq(mrb_state *mrb, const T_CDTQ *pcdtq)
{
  ER_ID id = acre_dtq(pcdtq);
  RTOS_VERIFY(mrb, id);
  return id;
}

ER RTOS_snd_dtq(mrb_state *mrb, ID id, intptr_t data, TMO tmout/*not use*/)
{
  ER er = snd_dtq(id, data);
  RTOS_VERIFY(mrb, er);
  return id;
}

ER RTOS_psnd_dtq(mrb_state *mrb, ID id, intptr_t data, TMO tmout/*not use*/)
{
  ER er = psnd_dtq(id, data);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return id;
}

ER RTOS_tsnd_dtq(mrb_state *mrb, ID id, intptr_t data, TMO tmout)
{
  ER er = tsnd_dtq(id, data, tmout);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return id;
}

ER RTOS_rcv_dtq(mrb_state *mrb, ID id, intptr_t *data, TMO tmout/*not use*/)
{
  ER er = rcv_dtq(id, data);
  RTOS_VERIFY(mrb, er);
  return id;
}

ER RTOS_prcv_dtq(mrb_state *mrb, ID id, intptr_t *data, TMO tmout/*not use*/)
{
  ER er = prcv_dtq(id, data);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return id;
}

ER RTOS_trcv_dtq(mrb_state *mrb, ID id, intptr_t *data, TMO tmout)
{
  ER er = trcv_dtq(id, data, tmout);
  RTOS_VERIFY_TIMEOUT(mrb, er);
  return id;
}
