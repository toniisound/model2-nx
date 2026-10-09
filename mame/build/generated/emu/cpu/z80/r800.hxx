if (m_wait_state)
{
	m_icount = 0; // stalled
	return;
}

const bool nomreq_en = !m_nomreq_cb.isunset();
[[maybe_unused]] const bool refresh_en = !m_refresh_cb.isunset();

bool interrupted = true;
while (u8(m_ref) != 0x00) {
// slow re-enter
	// workaround to simulate main loop behavior where continue statement relays on having it set after
	if (!interrupted) {
		m_ref = 0xffff00;
		continue;
	}
	interrupted = false;

switch (u8(m_ref >> 16)) // prefix
{
case 0x00:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x01:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x000101;
				} else {
					m_ref = 0x000102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x000103;
				} else {
					m_ref = 0x000104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			BC = TDAT;
		}
		continue;

	case 0x02:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT8 = A;
			[[fallthrough]];
		case 0x01:
			data_write(BC, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x000201;
				} else {
					m_ref = 0x000202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ_L = (BC + 1) & 0xFF;
			WZ_H = A;
		}
		continue;

	case 0x03:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x000301;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x000302;
				return;
			}
			[[fallthrough]];
		case 0x02:
			BC++;
		}
		continue;

	case 0x06:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x000601;
				} else {
					m_ref = 0x000602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			B = TDAT8;
		}
		continue;

	case 0x0a:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(BC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x000a01;
				} else {
					m_ref = 0x000a02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			A = TDAT8;
			WZ = BC+1;
		}
		continue;

	case 0x0b:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x000b01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x000b02;
				return;
			}
			[[fallthrough]];
		case 0x02:
			BC--;
		}
		continue;

	case 0x0e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x000e01;
				} else {
					m_ref = 0x000e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			C = TDAT8;
		}
		continue;

	case 0x10:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x001001;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (--B) {
			[[fallthrough]];
		case 0x02:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x001002;
				} else {
					m_ref = 0x001003;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			PC++;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x001004;
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC += (s8)TDAT8;
			WZ = PC;
			} else {
			[[fallthrough]];
		case 0x05:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x001005;
				} else {
					m_ref = 0x001006;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			PC++;
			}
		}
		continue;

	case 0x11:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x001101;
				} else {
					m_ref = 0x001102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x001103;
				} else {
					m_ref = 0x001104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			DE = TDAT;
		}
		continue;

	case 0x12:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT8 = A;
			[[fallthrough]];
		case 0x01:
			data_write(DE, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x001201;
				} else {
					m_ref = 0x001202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ_L = (DE + 1) & 0xFF;
			WZ_H = A;
		}
		continue;

	case 0x13:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x001301;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x001302;
				return;
			}
			[[fallthrough]];
		case 0x02:
			DE++;
		}
		continue;

	case 0x16:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x001601;
				} else {
					m_ref = 0x001602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			D = TDAT8;
		}
		continue;

	case 0x18:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x001801;
				} else {
					m_ref = 0x001802;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x001803;
				return;
			}
			[[fallthrough]];
		case 0x03:
			PC += (s8)TDAT8;
			WZ = PC;
		}
		continue;

	case 0x1a:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(DE);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x001a01;
				} else {
					m_ref = 0x001a02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			A = TDAT8;
			WZ = DE + 1;
		}
		continue;

	case 0x1b:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x001b01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x001b02;
				return;
			}
			[[fallthrough]];
		case 0x02:
			DE--;
		}
		continue;

	case 0x1e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x001e01;
				} else {
					m_ref = 0x001e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			E = TDAT8;
		}
		continue;

	case 0x20:
		switch (u8(m_ref))
		{
		case 0x00:
			if (!m_f.z()) {
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002001;
				} else {
					m_ref = 0x002002;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x002003;
				return;
			}
			[[fallthrough]];
		case 0x03:
			PC += (s8)TDAT8;
			WZ = PC;
			} else {
			[[fallthrough]];
		case 0x04:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002004;
				} else {
					m_ref = 0x002005;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC++;
			}
		}
		continue;

	case 0x21:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002101;
				} else {
					m_ref = 0x002102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002103;
				} else {
					m_ref = 0x002104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			HL = TDAT;
		}
		continue;

	case 0x22:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002201;
				} else {
					m_ref = 0x002202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002203;
				} else {
					m_ref = 0x002204;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			TDAT = HL;
			[[fallthrough]];
		case 0x05:
			data_write(m_ea, TDAT_L);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002205;
				} else {
					m_ref = 0x002206;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			data_write(m_ea+1, TDAT_H);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002207;
				} else {
					m_ref = 0x002208;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			WZ = m_ea + 1;
		}
		continue;

	case 0x23:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x002301;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x002302;
				return;
			}
			[[fallthrough]];
		case 0x02:
			HL++;
		}
		continue;

	case 0x26:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002601;
				} else {
					m_ref = 0x002602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			H = TDAT8;
		}
		continue;

	case 0x28:
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_f.z()) {
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002801;
				} else {
					m_ref = 0x002802;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x002803;
				return;
			}
			[[fallthrough]];
		case 0x03:
			PC += (s8)TDAT8;
			WZ = PC;
			} else {
			[[fallthrough]];
		case 0x04:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002804;
				} else {
					m_ref = 0x002805;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC++;
			}
		}
		continue;

	case 0x2a:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002a01;
				} else {
					m_ref = 0x002a02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002a03;
				} else {
					m_ref = 0x002a04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			[[fallthrough]];
		case 0x05:
			TDAT_L = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002a05;
				} else {
					m_ref = 0x002a06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			TDAT_H = data_read(m_ea+1);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002a07;
				} else {
					m_ref = 0x002a08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			HL = TDAT;
			WZ = m_ea + 1;
		}
		continue;

	case 0x2b:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x002b01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x002b02;
				return;
			}
			[[fallthrough]];
		case 0x02:
			HL--;
		}
		continue;

	case 0x2e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x002e01;
				} else {
					m_ref = 0x002e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			L = TDAT8;
		}
		continue;

	case 0x30:
		switch (u8(m_ref))
		{
		case 0x00:
			if (!m_f.c) {
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003001;
				} else {
					m_ref = 0x003002;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x003003;
				return;
			}
			[[fallthrough]];
		case 0x03:
			PC += (s8)TDAT8;
			WZ = PC;
			} else {
			[[fallthrough]];
		case 0x04:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003004;
				} else {
					m_ref = 0x003005;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC++;
			}
		}
		continue;

	case 0x31:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003101;
				} else {
					m_ref = 0x003102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003103;
				} else {
					m_ref = 0x003104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			SP = TDAT;
		}
		continue;

	case 0x32:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003201;
				} else {
					m_ref = 0x003202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003203;
				} else {
					m_ref = 0x003204;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			TDAT8 = A;
			[[fallthrough]];
		case 0x05:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003205;
				} else {
					m_ref = 0x003206;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			WZ_L = (m_ea + 1) & 0xFF;
			WZ_H = A;
		}
		continue;

	case 0x33:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x003301;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x003302;
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
		}
		continue;

	case 0x34:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003401;
				} else {
					m_ref = 0x003402;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x003403;
				return;
			}
			[[fallthrough]];
		case 0x03:
			inc(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003404;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x35:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003501;
				} else {
					m_ref = 0x003502;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x003503;
				return;
			}
			[[fallthrough]];
		case 0x03:
			dec(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003504;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x36:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003601;
				} else {
					m_ref = 0x003602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003603;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x38:
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_f.c) {
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003801;
				} else {
					m_ref = 0x003802;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x003803;
				return;
			}
			[[fallthrough]];
		case 0x03:
			PC += (s8)TDAT8;
			WZ = PC;
			} else {
			[[fallthrough]];
		case 0x04:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003804;
				} else {
					m_ref = 0x003805;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC++;
			}
		}
		continue;

	case 0x3a:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003a01;
				} else {
					m_ref = 0x003a02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003a03;
				} else {
					m_ref = 0x003a04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			[[fallthrough]];
		case 0x05:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003a05;
				} else {
					m_ref = 0x003a06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			A = TDAT8;
			WZ = m_ea + 1;
		}
		continue;

	case 0x3b:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x003b01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x003b02;
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP--;
		}
		continue;

	case 0x3e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x003e01;
				} else {
					m_ref = 0x003e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			A = TDAT8;
		}
		continue;

	case 0x46:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x004601;
				} else {
					m_ref = 0x004602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			B = TDAT8;
		}
		continue;

	case 0x4e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x004e01;
				} else {
					m_ref = 0x004e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			C = TDAT8;
		}
		continue;

	case 0x56:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x005601;
				} else {
					m_ref = 0x005602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			D = TDAT8;
		}
		continue;

	case 0x5e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x005e01;
				} else {
					m_ref = 0x005e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			E = TDAT8;
		}
		continue;

	case 0x66:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x006601;
				} else {
					m_ref = 0x006602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			H = TDAT8;
		}
		continue;

	case 0x6e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x006e01;
				} else {
					m_ref = 0x006e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			L = TDAT8;
		}
		continue;

	case 0x70:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT = B;
			[[fallthrough]];
		case 0x01:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x007001;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x71:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT = C;
			[[fallthrough]];
		case 0x01:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x007101;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x72:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT = D;
			[[fallthrough]];
		case 0x01:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x007201;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x73:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT = E;
			[[fallthrough]];
		case 0x01:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x007301;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x74:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT = H;
			[[fallthrough]];
		case 0x01:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x007401;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x75:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT = L;
			[[fallthrough]];
		case 0x01:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x007501;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x77:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT = A;
			[[fallthrough]];
		case 0x01:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x007701;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x7e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x007e01;
				} else {
					m_ref = 0x007e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			A = TDAT8;
		}
		continue;

	case 0x86:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x008601;
				} else {
					m_ref = 0x008602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			add_a(TDAT8);
		}
		continue;

	case 0x8e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x008e01;
				} else {
					m_ref = 0x008e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			adc_a(TDAT8);
		}
		continue;

	case 0x96:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x009601;
				} else {
					m_ref = 0x009602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			sub_a(TDAT8);
		}
		continue;

	case 0x9e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x009e01;
				} else {
					m_ref = 0x009e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			sbc_a(TDAT8);
		}
		continue;

	case 0xa6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00a601;
				} else {
					m_ref = 0x00a602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			and_a(TDAT8);
		}
		continue;

	case 0xae:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ae01;
				} else {
					m_ref = 0x00ae02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			xor_a(TDAT8);
		}
		continue;

	case 0xb6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00b601;
				} else {
					m_ref = 0x00b602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			or_a(TDAT8);
		}
		continue;

	case 0xbe:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00be01;
				} else {
					m_ref = 0x00be02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			cp(TDAT8);
		}
		continue;

	case 0xc0:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00c001;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (!m_f.z()) {
			[[fallthrough]];
		case 0x02:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c002;
				} else {
					m_ref = 0x00c003;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP++;
			[[fallthrough]];
		case 0x04:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c004;
				} else {
					m_ref = 0x00c005;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP++;
				PC = TDAT;
				WZ = PC;
			}
		}
		continue;

	case 0xc1:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c101;
				} else {
					m_ref = 0x00c102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c103;
				} else {
					m_ref = 0x00c104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			BC = TDAT;
		}
		continue;

	case 0xc2:
		switch (u8(m_ref))
		{
		case 0x00:
			if (!m_f.z()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c201;
				} else {
					m_ref = 0x00c202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c203;
				} else {
					m_ref = 0x00c204;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			PC = TDAT;
				WZ = PC;
			} else {
				// implicit do PC += 2
			[[fallthrough]];
		case 0x05:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c205;
				} else {
					m_ref = 0x00c206;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			PC++;
			[[fallthrough]];
		case 0x07:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c207;
				} else {
					m_ref = 0x00c208;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xc3:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c301;
				} else {
					m_ref = 0x00c302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c303;
				} else {
					m_ref = 0x00c304;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			PC = TDAT;
			WZ = PC;
		}
		continue;

	case 0xc4:
		switch (u8(m_ref))
		{
		case 0x00:
			if (!m_f.z()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c401;
				} else {
					m_ref = 0x00c402;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c403;
				} else {
					m_ref = 0x00c404;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00c405;
				return;
			}
			[[fallthrough]];
		case 0x05:
			WZ = m_ea;
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c406;
				} else {
					m_ref = 0x00c407;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c408;
				} else {
					m_ref = 0x00c409;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			PC = m_ea;
			} else {
			[[fallthrough]];
		case 0x0a:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c40a;
				} else {
					m_ref = 0x00c40b;
				}
				return;
			}
			[[fallthrough]];
		case 0x0b:
			PC++;
			[[fallthrough]];
		case 0x0c:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c40c;
				} else {
					m_ref = 0x00c40d;
				}
				return;
			}
			[[fallthrough]];
		case 0x0d:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xc5:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00c501;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, BC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c502;
				} else {
					m_ref = 0x00c503;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, BC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c504;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xc6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c601;
				} else {
					m_ref = 0x00c602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			add_a(TDAT8);
		}
		continue;

	case 0xc7:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00c701;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c702;
				} else {
					m_ref = 0x00c703;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c704;
				} else {
					m_ref = 0x00c705;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC = 0x00;
			WZ = PC;
		}
		continue;

	case 0xc8:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00c801;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (m_f.z()) {
			[[fallthrough]];
		case 0x02:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c802;
				} else {
					m_ref = 0x00c803;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP++;
			[[fallthrough]];
		case 0x04:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c804;
				} else {
					m_ref = 0x00c805;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP++;
				PC = TDAT;
				WZ = PC;
			}
		}
		continue;

	case 0xc9:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c901;
				} else {
					m_ref = 0x00c902;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00c903;
				} else {
					m_ref = 0x00c904;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			PC = TDAT;
			WZ = PC;
		}
		continue;

	case 0xca:
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_f.z()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ca01;
				} else {
					m_ref = 0x00ca02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ca03;
				} else {
					m_ref = 0x00ca04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			PC = TDAT;
				WZ = PC;
			} else {
				// implicit do PC += 2
			[[fallthrough]];
		case 0x05:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ca05;
				} else {
					m_ref = 0x00ca06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			PC++;
			[[fallthrough]];
		case 0x07:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ca07;
				} else {
					m_ref = 0x00ca08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xcb:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = opcode_read();
			m_icount -= m_m1_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_m1_cycles;
					m_ref = 0x00cb01;
				} else {
					m_ref = 0x00cb02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			R++;
			Q = QT;
			QT = YXF;
			m_ref = (0xcb << 16) | (TDAT8 << 8);
			goto process;
		}
		continue;

	case 0xcc:
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_f.z()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cc01;
				} else {
					m_ref = 0x00cc02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cc03;
				} else {
					m_ref = 0x00cc04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00cc05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			WZ = m_ea;
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cc06;
				} else {
					m_ref = 0x00cc07;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cc08;
				} else {
					m_ref = 0x00cc09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			PC = m_ea;
			} else {
			[[fallthrough]];
		case 0x0a:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cc0a;
				} else {
					m_ref = 0x00cc0b;
				}
				return;
			}
			[[fallthrough]];
		case 0x0b:
			PC++;
			[[fallthrough]];
		case 0x0c:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cc0c;
				} else {
					m_ref = 0x00cc0d;
				}
				return;
			}
			[[fallthrough]];
		case 0x0d:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xcd:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cd01;
				} else {
					m_ref = 0x00cd02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cd03;
				} else {
					m_ref = 0x00cd04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00cd05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			WZ = m_ea;
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cd06;
				} else {
					m_ref = 0x00cd07;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cd08;
				} else {
					m_ref = 0x00cd09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			PC = m_ea;
		}
		continue;

	case 0xce:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ce01;
				} else {
					m_ref = 0x00ce02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			adc_a(TDAT8);
		}
		continue;

	case 0xcf:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00cf01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cf02;
				} else {
					m_ref = 0x00cf03;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00cf04;
				} else {
					m_ref = 0x00cf05;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC = 0x08;
			WZ = PC;
		}
		continue;

	case 0xd0:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00d001;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (!m_f.c) {
			[[fallthrough]];
		case 0x02:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d002;
				} else {
					m_ref = 0x00d003;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP++;
			[[fallthrough]];
		case 0x04:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d004;
				} else {
					m_ref = 0x00d005;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP++;
				PC = TDAT;
				WZ = PC;
			}
		}
		continue;

	case 0xd1:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d101;
				} else {
					m_ref = 0x00d102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d103;
				} else {
					m_ref = 0x00d104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			DE = TDAT;
		}
		continue;

	case 0xd2:
		switch (u8(m_ref))
		{
		case 0x00:
			if (!m_f.c) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d201;
				} else {
					m_ref = 0x00d202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d203;
				} else {
					m_ref = 0x00d204;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			PC = TDAT;
				WZ = PC;
			} else {
				// implicit do PC += 2
			[[fallthrough]];
		case 0x05:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d205;
				} else {
					m_ref = 0x00d206;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			PC++;
			[[fallthrough]];
		case 0x07:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d207;
				} else {
					m_ref = 0x00d208;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xd3:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d301;
				} else {
					m_ref = 0x00d302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			TDAT2 = TDAT8 | (A << 8);
			TDAT = A;
			[[fallthrough]];
		case 0x03:
			m_io.write_interruptible(TDAT2, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0x00d303;
				} else {
					m_ref = 0x00d304;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			WZ_L = ((TDAT2 & 0xff) + 1) & 0xff;
			WZ_H = A;
		}
		continue;

	case 0xd4:
		switch (u8(m_ref))
		{
		case 0x00:
			if (!m_f.c) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d401;
				} else {
					m_ref = 0x00d402;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d403;
				} else {
					m_ref = 0x00d404;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00d405;
				return;
			}
			[[fallthrough]];
		case 0x05:
			WZ = m_ea;
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d406;
				} else {
					m_ref = 0x00d407;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d408;
				} else {
					m_ref = 0x00d409;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			PC = m_ea;
			} else {
			[[fallthrough]];
		case 0x0a:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d40a;
				} else {
					m_ref = 0x00d40b;
				}
				return;
			}
			[[fallthrough]];
		case 0x0b:
			PC++;
			[[fallthrough]];
		case 0x0c:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d40c;
				} else {
					m_ref = 0x00d40d;
				}
				return;
			}
			[[fallthrough]];
		case 0x0d:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xd5:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00d501;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, DE >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d502;
				} else {
					m_ref = 0x00d503;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, DE);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d504;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xd6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d601;
				} else {
					m_ref = 0x00d602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			sub_a(TDAT8);
		}
		continue;

	case 0xd7:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00d701;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d702;
				} else {
					m_ref = 0x00d703;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d704;
				} else {
					m_ref = 0x00d705;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC = 0x10;
			WZ = PC;
		}
		continue;

	case 0xd8:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00d801;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (m_f.c) {
			[[fallthrough]];
		case 0x02:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d802;
				} else {
					m_ref = 0x00d803;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP++;
			[[fallthrough]];
		case 0x04:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00d804;
				} else {
					m_ref = 0x00d805;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP++;
				PC = TDAT;
				WZ = PC;
			}
		}
		continue;

	case 0xda:
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_f.c) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00da01;
				} else {
					m_ref = 0x00da02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00da03;
				} else {
					m_ref = 0x00da04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			PC = TDAT;
				WZ = PC;
			} else {
				// implicit do PC += 2
			[[fallthrough]];
		case 0x05:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00da05;
				} else {
					m_ref = 0x00da06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			PC++;
			[[fallthrough]];
		case 0x07:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00da07;
				} else {
					m_ref = 0x00da08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xdb:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00db01;
				} else {
					m_ref = 0x00db02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			TDAT2 = TDAT8 | (A << 8);
			[[fallthrough]];
		case 0x03:
			TDAT8 = m_io.read_interruptible(TDAT2);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0x00db03;
				} else {
					m_ref = 0x00db04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			A = TDAT8;
			WZ = TDAT2 + 1;
		}
		continue;

	case 0xdc:
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_f.c) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00dc01;
				} else {
					m_ref = 0x00dc02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00dc03;
				} else {
					m_ref = 0x00dc04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00dc05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			WZ = m_ea;
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00dc06;
				} else {
					m_ref = 0x00dc07;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00dc08;
				} else {
					m_ref = 0x00dc09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			PC = m_ea;
			} else {
			[[fallthrough]];
		case 0x0a:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00dc0a;
				} else {
					m_ref = 0x00dc0b;
				}
				return;
			}
			[[fallthrough]];
		case 0x0b:
			PC++;
			[[fallthrough]];
		case 0x0c:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00dc0c;
				} else {
					m_ref = 0x00dc0d;
				}
				return;
			}
			[[fallthrough]];
		case 0x0d:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xdd:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = opcode_read();
			m_icount -= m_m1_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_m1_cycles;
					m_ref = 0x00dd01;
				} else {
					m_ref = 0x00dd02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			R++;
			Q = QT;
			QT = YXF;
			m_ref = (0xdd << 16) | (TDAT8 << 8);
			goto process;
		}
		continue;

	case 0xde:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00de01;
				} else {
					m_ref = 0x00de02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			sbc_a(TDAT8);
		}
		continue;

	case 0xdf:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00df01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00df02;
				} else {
					m_ref = 0x00df03;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00df04;
				} else {
					m_ref = 0x00df05;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC = 0x18;
			WZ = PC;
		}
		continue;

	case 0xe0:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00e001;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (!m_f.pv()) {
			[[fallthrough]];
		case 0x02:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e002;
				} else {
					m_ref = 0x00e003;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP++;
			[[fallthrough]];
		case 0x04:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e004;
				} else {
					m_ref = 0x00e005;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP++;
				PC = TDAT;
				WZ = PC;
			}
		}
		continue;

	case 0xe1:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e101;
				} else {
					m_ref = 0x00e102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e103;
				} else {
					m_ref = 0x00e104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			HL = TDAT;
		}
		continue;

	case 0xe2:
		switch (u8(m_ref))
		{
		case 0x00:
			if (!m_f.pv()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e201;
				} else {
					m_ref = 0x00e202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e203;
				} else {
					m_ref = 0x00e204;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			PC = TDAT;
				WZ = PC;
			} else {
				// implicit do PC += 2
			[[fallthrough]];
		case 0x05:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e205;
				} else {
					m_ref = 0x00e206;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			PC++;
			[[fallthrough]];
		case 0x07:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e207;
				} else {
					m_ref = 0x00e208;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xe3:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e301;
				} else {
					m_ref = 0x00e302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e303;
				} else {
					m_ref = 0x00e304;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			if (nomreq_en)
				m_nomreq_cb(SP-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00e305;
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, HL >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e306;
				} else {
					m_ref = 0x00e307;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e308;
				} else {
					m_ref = 0x00e309;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			HL = TDAT;
			if (nomreq_en)
				m_nomreq_cb(SP, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00e30a;
				return;
			}
			[[fallthrough]];
		case 0x0a:
			if (nomreq_en)
				m_nomreq_cb(SP, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00e30b;
				return;
			}
			[[fallthrough]];
		case 0x0b:
			WZ = TDAT;
		}
		continue;

	case 0xe4:
		switch (u8(m_ref))
		{
		case 0x00:
			if (!m_f.pv()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e401;
				} else {
					m_ref = 0x00e402;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e403;
				} else {
					m_ref = 0x00e404;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00e405;
				return;
			}
			[[fallthrough]];
		case 0x05:
			WZ = m_ea;
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e406;
				} else {
					m_ref = 0x00e407;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e408;
				} else {
					m_ref = 0x00e409;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			PC = m_ea;
			} else {
			[[fallthrough]];
		case 0x0a:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e40a;
				} else {
					m_ref = 0x00e40b;
				}
				return;
			}
			[[fallthrough]];
		case 0x0b:
			PC++;
			[[fallthrough]];
		case 0x0c:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e40c;
				} else {
					m_ref = 0x00e40d;
				}
				return;
			}
			[[fallthrough]];
		case 0x0d:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xe5:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00e501;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, HL >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e502;
				} else {
					m_ref = 0x00e503;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e504;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xe6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e601;
				} else {
					m_ref = 0x00e602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			and_a(TDAT8);
		}
		continue;

	case 0xe7:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00e701;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e702;
				} else {
					m_ref = 0x00e703;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e704;
				} else {
					m_ref = 0x00e705;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC = 0x20;
			WZ = PC;
		}
		continue;

	case 0xe8:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00e801;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (m_f.pv()) {
			[[fallthrough]];
		case 0x02:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e802;
				} else {
					m_ref = 0x00e803;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP++;
			[[fallthrough]];
		case 0x04:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00e804;
				} else {
					m_ref = 0x00e805;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP++;
				PC = TDAT;
				WZ = PC;
			}
		}
		continue;

	case 0xea:
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_f.pv()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ea01;
				} else {
					m_ref = 0x00ea02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ea03;
				} else {
					m_ref = 0x00ea04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			PC = TDAT;
				WZ = PC;
			} else {
				// implicit do PC += 2
			[[fallthrough]];
		case 0x05:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ea05;
				} else {
					m_ref = 0x00ea06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			PC++;
			[[fallthrough]];
		case 0x07:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ea07;
				} else {
					m_ref = 0x00ea08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xec:
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_f.pv()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ec01;
				} else {
					m_ref = 0x00ec02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ec03;
				} else {
					m_ref = 0x00ec04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00ec05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			WZ = m_ea;
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ec06;
				} else {
					m_ref = 0x00ec07;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ec08;
				} else {
					m_ref = 0x00ec09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			PC = m_ea;
			} else {
			[[fallthrough]];
		case 0x0a:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ec0a;
				} else {
					m_ref = 0x00ec0b;
				}
				return;
			}
			[[fallthrough]];
		case 0x0b:
			PC++;
			[[fallthrough]];
		case 0x0c:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ec0c;
				} else {
					m_ref = 0x00ec0d;
				}
				return;
			}
			[[fallthrough]];
		case 0x0d:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xed:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = opcode_read();
			m_icount -= m_m1_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_m1_cycles;
					m_ref = 0x00ed01;
				} else {
					m_ref = 0x00ed02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			R++;
			Q = QT;
			QT = YXF;
			m_ref = (0xed << 16) | (TDAT8 << 8);
			goto process;
		}
		continue;

	case 0xee:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ee01;
				} else {
					m_ref = 0x00ee02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			xor_a(TDAT8);
		}
		continue;

	case 0xef:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00ef01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ef02;
				} else {
					m_ref = 0x00ef03;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ef04;
				} else {
					m_ref = 0x00ef05;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC = 0x28;
			WZ = PC;
		}
		continue;

	case 0xf0:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00f001;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (!m_f.s()) {
			[[fallthrough]];
		case 0x02:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f002;
				} else {
					m_ref = 0x00f003;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP++;
			[[fallthrough]];
		case 0x04:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f004;
				} else {
					m_ref = 0x00f005;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP++;
				PC = TDAT;
				WZ = PC;
			}
		}
		continue;

	case 0xf1:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f101;
				} else {
					m_ref = 0x00f102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f103;
				} else {
					m_ref = 0x00f104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			A = TDAT_H;
			set_f(TDAT_L);
		}
		continue;

	case 0xf2:
		switch (u8(m_ref))
		{
		case 0x00:
			if (!m_f.s()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f201;
				} else {
					m_ref = 0x00f202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f203;
				} else {
					m_ref = 0x00f204;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			PC = TDAT;
				WZ = PC;
			} else {
				// implicit do PC += 2
			[[fallthrough]];
		case 0x05:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f205;
				} else {
					m_ref = 0x00f206;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			PC++;
			[[fallthrough]];
		case 0x07:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f207;
				} else {
					m_ref = 0x00f208;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xf3:
		switch (u8(m_ref))
		{
		case 0x00:
			m_iff1 = m_iff2 = 0;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0xf4:
		switch (u8(m_ref))
		{
		case 0x00:
			if (!m_f.s()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f401;
				} else {
					m_ref = 0x00f402;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f403;
				} else {
					m_ref = 0x00f404;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00f405;
				return;
			}
			[[fallthrough]];
		case 0x05:
			WZ = m_ea;
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f406;
				} else {
					m_ref = 0x00f407;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f408;
				} else {
					m_ref = 0x00f409;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			PC = m_ea;
			} else {
			[[fallthrough]];
		case 0x0a:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f40a;
				} else {
					m_ref = 0x00f40b;
				}
				return;
			}
			[[fallthrough]];
		case 0x0b:
			PC++;
			[[fallthrough]];
		case 0x0c:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f40c;
				} else {
					m_ref = 0x00f40d;
				}
				return;
			}
			[[fallthrough]];
		case 0x0d:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xf5:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00f501;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, A);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f502;
				} else {
					m_ref = 0x00f503;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, get_f());
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f504;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xf6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f601;
				} else {
					m_ref = 0x00f602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			or_a(TDAT8);
		}
		continue;

	case 0xf7:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00f701;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f702;
				} else {
					m_ref = 0x00f703;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f704;
				} else {
					m_ref = 0x00f705;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC = 0x30;
			WZ = PC;
		}
		continue;

	case 0xf8:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00f801;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (m_f.s()) {
			[[fallthrough]];
		case 0x02:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f802;
				} else {
					m_ref = 0x00f803;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP++;
			[[fallthrough]];
		case 0x04:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00f804;
				} else {
					m_ref = 0x00f805;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP++;
				PC = TDAT;
				WZ = PC;
			}
		}
		continue;

	case 0xf9:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00f901;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00f902;
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP = HL;
		}
		continue;

	case 0xfa:
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_f.s()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fa01;
				} else {
					m_ref = 0x00fa02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fa03;
				} else {
					m_ref = 0x00fa04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			PC = TDAT;
				WZ = PC;
			} else {
				// implicit do PC += 2
			[[fallthrough]];
		case 0x05:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fa05;
				} else {
					m_ref = 0x00fa06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			PC++;
			[[fallthrough]];
		case 0x07:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fa07;
				} else {
					m_ref = 0x00fa08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xfc:
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_f.s()) {
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fc01;
				} else {
					m_ref = 0x00fc02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fc03;
				} else {
					m_ref = 0x00fc04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00fc05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			WZ = m_ea;
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fc06;
				} else {
					m_ref = 0x00fc07;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fc08;
				} else {
					m_ref = 0x00fc09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			PC = m_ea;
			} else {
			[[fallthrough]];
		case 0x0a:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fc0a;
				} else {
					m_ref = 0x00fc0b;
				}
				return;
			}
			[[fallthrough]];
		case 0x0b:
			PC++;
			[[fallthrough]];
		case 0x0c:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fc0c;
				} else {
					m_ref = 0x00fc0d;
				}
				return;
			}
			[[fallthrough]];
		case 0x0d:
			PC++;
			WZ = TDAT;
			}
		}
		continue;

	case 0xfd:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = opcode_read();
			m_icount -= m_m1_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_m1_cycles;
					m_ref = 0x00fd01;
				} else {
					m_ref = 0x00fd02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			R++;
			Q = QT;
			QT = YXF;
			m_ref = (0xfd << 16) | (TDAT8 << 8);
			goto process;
		}
		continue;

	case 0xfe:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00fe01;
				} else {
					m_ref = 0x00fe02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			cp(TDAT8);
		}
		continue;

	case 0xff:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0x00ff01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ff02;
				} else {
					m_ref = 0x00ff03;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0x00ff04;
				} else {
					m_ref = 0x00ff05;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC = 0x38;
			WZ = PC;
		}
		continue;

	}
} break; // prefix: 00

case 0xcb:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x06:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb0601;
				} else {
					m_ref = 0xcb0602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb0603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = rlc(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb0604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x0e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb0e01;
				} else {
					m_ref = 0xcb0e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb0e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = rrc(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb0e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x16:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb1601;
				} else {
					m_ref = 0xcb1602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb1603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = rl(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb1604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x1e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb1e01;
				} else {
					m_ref = 0xcb1e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb1e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = rr(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb1e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x26:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb2601;
				} else {
					m_ref = 0xcb2602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb2603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = sla(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb2604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x2e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb2e01;
				} else {
					m_ref = 0xcb2e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb2e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = sra(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb2e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x36:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb3601;
				} else {
					m_ref = 0xcb3602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb3603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = r800_sll(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb3604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x3e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb3e01;
				} else {
					m_ref = 0xcb3e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb3e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = srl(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb3e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x46:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb4601;
				} else {
					m_ref = 0xcb4602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb4603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_hl(0, TDAT8);
		}
		continue;

	case 0x4e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb4e01;
				} else {
					m_ref = 0xcb4e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb4e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_hl(1, TDAT8);
		}
		continue;

	case 0x56:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb5601;
				} else {
					m_ref = 0xcb5602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb5603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_hl(2, TDAT8);
		}
		continue;

	case 0x5e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb5e01;
				} else {
					m_ref = 0xcb5e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb5e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_hl(3, TDAT8);
		}
		continue;

	case 0x66:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb6601;
				} else {
					m_ref = 0xcb6602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb6603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_hl(4, TDAT8);
		}
		continue;

	case 0x6e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb6e01;
				} else {
					m_ref = 0xcb6e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb6e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_hl(5, TDAT8);
		}
		continue;

	case 0x76:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb7601;
				} else {
					m_ref = 0xcb7602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb7603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_hl(6, TDAT8);
		}
		continue;

	case 0x7e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb7e01;
				} else {
					m_ref = 0xcb7e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb7e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_hl(7, TDAT8);
		}
		continue;

	case 0x86:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb8601;
				} else {
					m_ref = 0xcb8602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb8603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(0, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb8604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x8e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb8e01;
				} else {
					m_ref = 0xcb8e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb8e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(1, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb8e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x96:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb9601;
				} else {
					m_ref = 0xcb9602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb9603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(2, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb9604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x9e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb9e01;
				} else {
					m_ref = 0xcb9e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcb9e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(3, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcb9e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xa6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcba601;
				} else {
					m_ref = 0xcba602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcba603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(4, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcba604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xae:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbae01;
				} else {
					m_ref = 0xcbae02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbae03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(5, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbae04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xb6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbb601;
				} else {
					m_ref = 0xcbb602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbb603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(6, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbb604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xbe:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbbe01;
				} else {
					m_ref = 0xcbbe02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbbe03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(7, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbbe04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xc6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbc601;
				} else {
					m_ref = 0xcbc602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbc603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(0, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbc604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xce:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbce01;
				} else {
					m_ref = 0xcbce02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbce03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(1, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbce04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xd6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbd601;
				} else {
					m_ref = 0xcbd602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbd603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(2, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbd604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xde:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbde01;
				} else {
					m_ref = 0xcbde02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbde03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(3, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbde04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xe6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbe601;
				} else {
					m_ref = 0xcbe602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbe603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(4, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbe604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xee:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbee01;
				} else {
					m_ref = 0xcbee02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbee03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(5, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbee04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xf6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbf601;
				} else {
					m_ref = 0xcbf602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbf603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(6, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbf604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xfe:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbfe01;
				} else {
					m_ref = 0xcbfe02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xcbfe03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(7, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xcbfe04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	}
} break; // prefix: cb

case 0xdd:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x21:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2101;
				} else {
					m_ref = 0xdd2102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2103;
				} else {
					m_ref = 0xdd2104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			IX = TDAT;
		}
		continue;

	case 0x22:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2201;
				} else {
					m_ref = 0xdd2202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2203;
				} else {
					m_ref = 0xdd2204;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			TDAT = IX;
			[[fallthrough]];
		case 0x05:
			data_write(m_ea, TDAT_L);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2205;
				} else {
					m_ref = 0xdd2206;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			data_write(m_ea+1, TDAT_H);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2207;
				} else {
					m_ref = 0xdd2208;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			WZ = m_ea + 1;
		}
		continue;

	case 0x23:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd2301;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd2302;
				return;
			}
			[[fallthrough]];
		case 0x02:
			IX++;
		}
		continue;

	case 0x26:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2601;
				} else {
					m_ref = 0xdd2602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			HX = TDAT8;
		}
		continue;

	case 0x2a:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2a01;
				} else {
					m_ref = 0xdd2a02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2a03;
				} else {
					m_ref = 0xdd2a04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			[[fallthrough]];
		case 0x05:
			TDAT_L = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2a05;
				} else {
					m_ref = 0xdd2a06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			TDAT_H = data_read(m_ea+1);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2a07;
				} else {
					m_ref = 0xdd2a08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			IX = TDAT;
			WZ = m_ea + 1;
		}
		continue;

	case 0x2b:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd2b01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd2b02;
				return;
			}
			[[fallthrough]];
		case 0x02:
			IX--;
		}
		continue;

	case 0x2e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd2e01;
				} else {
					m_ref = 0xdd2e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			LX = TDAT8;
		}
		continue;

	case 0x34:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd3401;
				} else {
					m_ref = 0xdd3402;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3403;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3404;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3405;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3406;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3407;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd3408;
				} else {
					m_ref = 0xdd3409;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd340a;
				return;
			}
			[[fallthrough]];
		case 0x0a:
			inc(TDAT8);
			[[fallthrough]];
		case 0x0b:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd340b;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x35:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd3501;
				} else {
					m_ref = 0xdd3502;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3503;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3504;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3505;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3506;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3507;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd3508;
				} else {
					m_ref = 0xdd3509;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd350a;
				return;
			}
			[[fallthrough]];
		case 0x0a:
			dec(TDAT8);
			[[fallthrough]];
		case 0x0b:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd350b;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x36:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd3601;
				} else {
					m_ref = 0xdd3602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			[[fallthrough]];
		case 0x03:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd3603;
				} else {
					m_ref = 0xdd3604;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd3606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd3607;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x46:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd4601;
				} else {
					m_ref = 0xdd4602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd4603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd4604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd4605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd4606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd4607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd4608;
				} else {
					m_ref = 0xdd4609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			B = TDAT8;
		}
		continue;

	case 0x4e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd4e01;
				} else {
					m_ref = 0xdd4e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd4e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd4e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd4e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd4e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd4e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd4e08;
				} else {
					m_ref = 0xdd4e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			C = TDAT8;
		}
		continue;

	case 0x56:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd5601;
				} else {
					m_ref = 0xdd5602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd5603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd5604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd5605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd5606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd5607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd5608;
				} else {
					m_ref = 0xdd5609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			D = TDAT8;
		}
		continue;

	case 0x5e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd5e01;
				} else {
					m_ref = 0xdd5e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd5e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd5e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd5e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd5e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd5e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd5e08;
				} else {
					m_ref = 0xdd5e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			E = TDAT8;
		}
		continue;

	case 0x66:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd6601;
				} else {
					m_ref = 0xdd6602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd6603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd6604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd6605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd6606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd6607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd6608;
				} else {
					m_ref = 0xdd6609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			H = TDAT8;
		}
		continue;

	case 0x6e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd6e01;
				} else {
					m_ref = 0xdd6e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd6e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd6e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd6e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd6e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd6e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd6e08;
				} else {
					m_ref = 0xdd6e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			L = TDAT8;
		}
		continue;

	case 0x70:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7001;
				} else {
					m_ref = 0xdd7002;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7003;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7004;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7005;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7006;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7007;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = B;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7008;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x71:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7101;
				} else {
					m_ref = 0xdd7102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7103;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7104;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7105;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7106;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7107;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = C;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7108;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x72:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7201;
				} else {
					m_ref = 0xdd7202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7203;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7204;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7205;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7206;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7207;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = D;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7208;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x73:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7301;
				} else {
					m_ref = 0xdd7302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7303;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7304;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7305;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7306;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7307;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = E;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7308;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x74:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7401;
				} else {
					m_ref = 0xdd7402;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7403;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7404;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7405;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7406;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7407;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = H;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7408;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x75:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7501;
				} else {
					m_ref = 0xdd7502;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7503;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7504;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7505;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7506;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7507;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = L;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7508;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x77:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7701;
				} else {
					m_ref = 0xdd7702;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7703;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7704;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7705;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7706;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7707;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = A;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7708;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x7e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7e01;
				} else {
					m_ref = 0xdd7e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd7e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd7e08;
				} else {
					m_ref = 0xdd7e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			A = TDAT8;
		}
		continue;

	case 0x86:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd8601;
				} else {
					m_ref = 0xdd8602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd8603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd8604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd8605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd8606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd8607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd8608;
				} else {
					m_ref = 0xdd8609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			add_a(TDAT8);
		}
		continue;

	case 0x8e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd8e01;
				} else {
					m_ref = 0xdd8e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd8e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd8e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd8e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd8e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd8e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd8e08;
				} else {
					m_ref = 0xdd8e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			adc_a(TDAT8);
		}
		continue;

	case 0x96:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd9601;
				} else {
					m_ref = 0xdd9602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd9603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd9604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd9605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd9606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd9607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd9608;
				} else {
					m_ref = 0xdd9609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			sub_a(TDAT8);
		}
		continue;

	case 0x9e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd9e01;
				} else {
					m_ref = 0xdd9e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd9e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd9e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd9e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd9e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdd9e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdd9e08;
				} else {
					m_ref = 0xdd9e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			sbc_a(TDAT8);
		}
		continue;

	case 0xa6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdda601;
				} else {
					m_ref = 0xdda602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdda603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdda604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdda605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdda606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdda607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdda608;
				} else {
					m_ref = 0xdda609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			and_a(TDAT8);
		}
		continue;

	case 0xae:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xddae01;
				} else {
					m_ref = 0xddae02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddae03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddae04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddae05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddae06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddae07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xddae08;
				} else {
					m_ref = 0xddae09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			xor_a(TDAT8);
		}
		continue;

	case 0xb6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xddb601;
				} else {
					m_ref = 0xddb602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddb603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddb604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddb605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddb606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddb607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xddb608;
				} else {
					m_ref = 0xddb609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			or_a(TDAT8);
		}
		continue;

	case 0xbe:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xddbe01;
				} else {
					m_ref = 0xddbe02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddbe03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddbe04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddbe05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddbe06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddbe07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xddbe08;
				} else {
					m_ref = 0xddbe09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			cp(TDAT8);
		}
		continue;

	case 0xcb:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xddcb01;
				} else {
					m_ref = 0xddcb02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IX + (s8)TDAT8);
			WZ = m_ea;
			[[fallthrough]];
		case 0x03:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xddcb03;
				} else {
					m_ref = 0xddcb04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddcb05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddcb06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			m_ref = (0xfe << 16) | (TDAT8 << 8);
			goto process;
		}
		continue;

	case 0xe1:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdde101;
				} else {
					m_ref = 0xdde102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdde103;
				} else {
					m_ref = 0xdde104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			IX = TDAT;
		}
		continue;

	case 0xe3:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdde301;
				} else {
					m_ref = 0xdde302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdde303;
				} else {
					m_ref = 0xdde304;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			if (nomreq_en)
				m_nomreq_cb(SP-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdde305;
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, IX >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdde306;
				} else {
					m_ref = 0xdde307;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, IX);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdde308;
				} else {
					m_ref = 0xdde309;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			IX = TDAT;
			if (nomreq_en)
				m_nomreq_cb(SP, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdde30a;
				return;
			}
			[[fallthrough]];
		case 0x0a:
			if (nomreq_en)
				m_nomreq_cb(SP, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdde30b;
				return;
			}
			[[fallthrough]];
		case 0x0b:
			WZ = TDAT;
		}
		continue;

	case 0xe5:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xdde501;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, IX >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdde502;
				} else {
					m_ref = 0xdde503;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, IX);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xdde504;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xf9:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddf901;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xddf902;
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP = IX;
		}
		continue;

	}
} break; // prefix: dd

case 0xed:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x40:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed4001;
				} else {
					m_ref = 0xed4002;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
				m_f.yx_val = TDAT8;
				m_f.h_val = m_f.n = 0;
			}
			B = TDAT8;
			WZ = BC + 1;
		}
		continue;

	case 0x41:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT8 = B;
			[[fallthrough]];
		case 0x01:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed4101;
				} else {
					m_ref = 0xed4102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = BC + 1;
		}
		continue;

	case 0x43:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4301;
				} else {
					m_ref = 0xed4302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4303;
				} else {
					m_ref = 0xed4304;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			TDAT = BC;
			[[fallthrough]];
		case 0x05:
			data_write(m_ea, TDAT_L);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4305;
				} else {
					m_ref = 0xed4306;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			data_write(m_ea+1, TDAT_H);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4307;
				} else {
					m_ref = 0xed4308;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			WZ = m_ea + 1;
		}
		continue;

	case 0x45:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4501;
				} else {
					m_ref = 0xed4502;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4503;
				} else {
					m_ref = 0xed4504;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			PC = TDAT;
			LOGMASKED(LOG_INT, "RETN m_iff1:%d m_iff2:%d\n", m_iff1, m_iff2);
			WZ = PC;
			m_iff1 = m_iff2;
		}
		continue;

	case 0x46:
		switch (u8(m_ref))
		{
		case 0x00:
			m_im = 0;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0x48:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed4801;
				} else {
					m_ref = 0xed4802;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
				m_f.yx_val = TDAT8;
				m_f.h_val = m_f.n = 0;
			}
			C = TDAT8;
			WZ = BC + 1;
		}
		continue;

	case 0x49:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT8 = C;
			[[fallthrough]];
		case 0x01:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed4901;
				} else {
					m_ref = 0xed4902;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = BC + 1;
		}
		continue;

	case 0x4b:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4b01;
				} else {
					m_ref = 0xed4b02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4b03;
				} else {
					m_ref = 0xed4b04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			[[fallthrough]];
		case 0x05:
			TDAT_L = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4b05;
				} else {
					m_ref = 0xed4b06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			TDAT_H = data_read(m_ea+1);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4b07;
				} else {
					m_ref = 0xed4b08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			BC = TDAT;
			WZ = m_ea + 1;
		}
		continue;

	case 0x4d:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4d01;
				} else {
					m_ref = 0xed4d02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed4d03;
				} else {
					m_ref = 0xed4d04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			PC = TDAT;
			WZ = PC;
			m_iff1 = m_iff2;
			daisy_call_reti_device();
		}
		continue;

	case 0x50:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed5001;
				} else {
					m_ref = 0xed5002;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
				m_f.yx_val = TDAT8;
				m_f.h_val = m_f.n = 0;
			}
			D = TDAT8;
			WZ = BC + 1;
		}
		continue;

	case 0x51:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT8 = D;
			[[fallthrough]];
		case 0x01:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed5101;
				} else {
					m_ref = 0xed5102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = BC + 1;
		}
		continue;

	case 0x53:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed5301;
				} else {
					m_ref = 0xed5302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed5303;
				} else {
					m_ref = 0xed5304;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			TDAT = DE;
			[[fallthrough]];
		case 0x05:
			data_write(m_ea, TDAT_L);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed5305;
				} else {
					m_ref = 0xed5306;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			data_write(m_ea+1, TDAT_H);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed5307;
				} else {
					m_ref = 0xed5308;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			WZ = m_ea + 1;
		}
		continue;

	case 0x56:
		switch (u8(m_ref))
		{
		case 0x00:
			m_im = 1;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0x58:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed5801;
				} else {
					m_ref = 0xed5802;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
				m_f.yx_val = TDAT8;
				m_f.h_val = m_f.n = 0;
			}
			E = TDAT8;
			WZ = BC + 1;
		}
		continue;

	case 0x59:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT8 = E;
			[[fallthrough]];
		case 0x01:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed5901;
				} else {
					m_ref = 0xed5902;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = BC + 1;
		}
		continue;

	case 0x5b:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed5b01;
				} else {
					m_ref = 0xed5b02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed5b03;
				} else {
					m_ref = 0xed5b04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			[[fallthrough]];
		case 0x05:
			TDAT_L = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed5b05;
				} else {
					m_ref = 0xed5b06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			TDAT_H = data_read(m_ea+1);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed5b07;
				} else {
					m_ref = 0xed5b08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			DE = TDAT;
			WZ = m_ea + 1;
		}
		continue;

	case 0x5e:
		switch (u8(m_ref))
		{
		case 0x00:
			m_im = 2;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0x60:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed6001;
				} else {
					m_ref = 0xed6002;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
				m_f.yx_val = TDAT8;
				m_f.h_val = m_f.n = 0;
			}
			H = TDAT8;
			WZ = BC + 1;
		}
		continue;

	case 0x61:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT8 = H;
			[[fallthrough]];
		case 0x01:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed6101;
				} else {
					m_ref = 0xed6102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = BC + 1;
		}
		continue;

	case 0x63:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6301;
				} else {
					m_ref = 0xed6302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6303;
				} else {
					m_ref = 0xed6304;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			TDAT = HL;
			[[fallthrough]];
		case 0x05:
			data_write(m_ea, TDAT_L);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6305;
				} else {
					m_ref = 0xed6306;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			data_write(m_ea+1, TDAT_H);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6307;
				} else {
					m_ref = 0xed6308;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			WZ = m_ea + 1;
		}
		continue;

	case 0x67:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6701;
				} else {
					m_ref = 0xed6702;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = HL+1;
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xed6703;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xed6704;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xed6705;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xed6706;
				return;
			}
			[[fallthrough]];
		case 0x06:
			TDAT_H = TDAT8;
			TDAT8 = (TDAT8 >> 4) | (A << 4);
			[[fallthrough]];
		case 0x07:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6707;
				} else {
					m_ref = 0xed6708;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			A = (A & 0xf0) | (TDAT_H & 0x0f);
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = m_f.pv_val = A;
				m_f.yx_val = A;
				m_f.h_val = m_f.n = 0;
			}
		}
		continue;

	case 0x68:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed6801;
				} else {
					m_ref = 0xed6802;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
				m_f.yx_val = TDAT8;
				m_f.h_val = m_f.n = 0;
			}
			L = TDAT8;
			WZ = BC + 1;
		}
		continue;

	case 0x69:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT8 = L;
			[[fallthrough]];
		case 0x01:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed6901;
				} else {
					m_ref = 0xed6902;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = BC + 1;
		}
		continue;

	case 0x6b:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6b01;
				} else {
					m_ref = 0xed6b02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6b03;
				} else {
					m_ref = 0xed6b04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			[[fallthrough]];
		case 0x05:
			TDAT_L = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6b05;
				} else {
					m_ref = 0xed6b06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			TDAT_H = data_read(m_ea+1);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6b07;
				} else {
					m_ref = 0xed6b08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			HL = TDAT;
			WZ = m_ea + 1;
		}
		continue;

	case 0x6d:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6d01;
				} else {
					m_ref = 0xed6d02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6d03;
				} else {
					m_ref = 0xed6d04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			PC = TDAT;
			WZ = PC;
			m_iff1 = m_iff2;
			daisy_call_reti_device();
		}
		continue;

	case 0x6e:
		switch (u8(m_ref))
		{
		case 0x00:
			m_im = 0;
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0x6f:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6f01;
				} else {
					m_ref = 0xed6f02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = HL+1;
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xed6f03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xed6f04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xed6f05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xed6f06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			TDAT_H = TDAT8;
			TDAT8 = (TDAT8 << 4) | (A & 0x0f);
			[[fallthrough]];
		case 0x07:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed6f07;
				} else {
					m_ref = 0xed6f08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			A = (A & 0xf0) | (TDAT_H >> 4);
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = m_f.pv_val = A;
				m_f.yx_val = A;
				m_f.h_val = m_f.n = 0;
			}
		}
		continue;

	case 0x70:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed7001;
				} else {
					m_ref = 0xed7002;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
				m_f.yx_val = TDAT8;
				m_f.h_val = m_f.n = 0;
			}
			WZ = BC + 1;
		}
		continue;

	case 0x73:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed7301;
				} else {
					m_ref = 0xed7302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed7303;
				} else {
					m_ref = 0xed7304;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			TDAT = SP;
			[[fallthrough]];
		case 0x05:
			data_write(m_ea, TDAT_L);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed7305;
				} else {
					m_ref = 0xed7306;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			data_write(m_ea+1, TDAT_H);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed7307;
				} else {
					m_ref = 0xed7308;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			WZ = m_ea + 1;
		}
		continue;

	case 0x78:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed7801;
				} else {
					m_ref = 0xed7802;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
				m_f.yx_val = TDAT8;
				m_f.h_val = m_f.n = 0;
			}
			A = TDAT8;
			WZ = BC + 1;
		}
		continue;

	case 0x79:
		switch (u8(m_ref))
		{
		case 0x00:
			TDAT8 = A;
			[[fallthrough]];
		case 0x01:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xed7901;
				} else {
					m_ref = 0xed7902;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = BC + 1;
		}
		continue;

	case 0x7b:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed7b01;
				} else {
					m_ref = 0xed7b02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed7b03;
				} else {
					m_ref = 0xed7b04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			[[fallthrough]];
		case 0x05:
			TDAT_L = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed7b05;
				} else {
					m_ref = 0xed7b06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			TDAT_H = data_read(m_ea+1);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xed7b07;
				} else {
					m_ref = 0xed7b08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			SP = TDAT;
			WZ = m_ea + 1;
		}
		continue;

	case 0xa0:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xeda001;
				} else {
					m_ref = 0xeda002;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			[[fallthrough]];
		case 0x03:
			data_write(DE, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xeda003;
				} else {
					m_ref = 0xeda004;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(DE, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xeda005;
				return;
			}
			[[fallthrough]];
		case 0x05:
			HL++;
			DE++;
			BC--;
			{
				QT = 0;
				// keep SZC
				m_f.yx_val = ((A + TDAT8) << 4) | ((A + TDAT8) & 0x0f);
				m_f.h_val = m_f.n = 0;
				m_f.pv_val = !BC;
			}
		}
		continue;

	case 0xa1:
		switch (u8(m_ref))
		{
		case 0x00:
		{
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xeda101;
				} else {
					m_ref = 0xeda102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xeda103;
				return;
			}
			[[fallthrough]];
		case 0x03:
			WZ++;
			HL++;
			BC--;
			u8 res = A - TDAT8;
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = res;
				m_f.h_val = A ^ TDAT8 ^ res;
				if (m_f.h())
					res -= 1;
				m_f.yx_val = (res << 4) | (res & 0x0f);
				m_f.pv_val = !BC;
				m_f.n = 1;
			}
		}
		}
		continue;

	case 0xa2:
		switch (u8(m_ref))
		{
		case 0x00:
		{
			[[fallthrough]];
		case 0x01:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xeda201;
				} else {
					m_ref = 0xeda202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = BC + 1;
			B--;
			[[fallthrough]];
		case 0x03:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xeda203;
				} else {
					m_ref = 0xeda204;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			HL++;
			const u16 t = ((C + 1) & 0xff) + TDAT8;
			{
				QT = 0;
				m_f.s_val = m_f.z_val = B;
				m_f.yx_val = B;
				m_f.h_val = t >> 4;
				m_f.pv_val = (t & 0x07) ^ B;
				m_f.n = TDAT8 & SF;
				m_f.c = t & 0x100;
			}
		}
		}
		continue;

	case 0xa3:
		switch (u8(m_ref))
		{
		case 0x00:
		{
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xeda301;
				} else {
					m_ref = 0xeda302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			B--;
			WZ = BC + 1;
			[[fallthrough]];
		case 0x03:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xeda303;
				} else {
					m_ref = 0xeda304;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			HL++;
			const u16 t = L + TDAT8;
			{
				QT = 0;
				m_f.s_val = m_f.z_val = B;
				m_f.yx_val = B;
				m_f.h_val = t >> 4;
				m_f.pv_val = (t & 0x07) ^ B;
				m_f.n = TDAT8 & SF;
				m_f.c = t & 0x100;
			}
		}
		}
		continue;

	case 0xa8:
		switch (u8(m_ref))
		{
		case 0x00:
		{
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xeda801;
				} else {
					m_ref = 0xeda802;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			[[fallthrough]];
		case 0x03:
			data_write(DE, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xeda803;
				} else {
					m_ref = 0xeda804;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(DE, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xeda805;
				return;
			}
			[[fallthrough]];
		case 0x05:
			HL--;
			DE--;
			BC--;
			{
				QT = 0;
				// keep SZC
				m_f.yx_val = ((A + TDAT8) << 4) | ((A + TDAT8) & 0x0f);
				m_f.h_val = m_f.n = 0;
				m_f.pv_val = !BC;
			}
		}
		}
		continue;

	case 0xa9:
		switch (u8(m_ref))
		{
		case 0x00:
		{
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xeda901;
				} else {
					m_ref = 0xeda902;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xeda903;
				return;
			}
			[[fallthrough]];
		case 0x03:
			WZ--;
			HL--;
			BC--;
			u8 res = A - TDAT8;
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = res;
				m_f.h_val = A ^ TDAT8 ^ res;
				if (m_f.h())
					res -= 1;
				m_f.yx_val = (res << 4) | (res & 0x0f);
				m_f.pv_val = !BC;
				m_f.n = 1;
			}
		}
		}
		continue;

	case 0xaa:
		switch (u8(m_ref))
		{
		case 0x00:
		{
			[[fallthrough]];
		case 0x01:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xedaa01;
				} else {
					m_ref = 0xedaa02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			WZ = BC - 1;
			B--;
			[[fallthrough]];
		case 0x03:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedaa03;
				} else {
					m_ref = 0xedaa04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			HL--;
			const u16 t = ((C - 1) & 0xff) + TDAT8;
			{
				QT = 0;
				m_f.s_val = m_f.z_val = B;
				m_f.yx_val = B;
				m_f.h_val = t >> 4;
				m_f.pv_val = (t & 0x07) ^ B;
				m_f.n = TDAT8 & SF;
				m_f.c = t & 0x100;
			}
		}
		}
		continue;

	case 0xab:
		switch (u8(m_ref))
		{
		case 0x00:
		{
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedab01;
				} else {
					m_ref = 0xedab02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			B--;
			WZ = BC - 1;
			[[fallthrough]];
		case 0x03:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xedab03;
				} else {
					m_ref = 0xedab04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			HL--;
			const u16 t = L + TDAT8;
			{
				QT = 0;
				m_f.s_val = m_f.z_val = B;
				m_f.yx_val = B;
				m_f.h_val = t >> 4;
				m_f.pv_val = (t & 0x07) ^ B;
				m_f.n = TDAT8 & SF;
				m_f.c = t & 0x100;
			}
		}
		}
		continue;

	case 0xb0:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedb001;
				} else {
					m_ref = 0xedb002;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			[[fallthrough]];
		case 0x03:
			data_write(DE, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedb003;
				} else {
					m_ref = 0xedb004;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(DE, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb005;
				return;
			}
			[[fallthrough]];
		case 0x05:
			HL++;
			DE++;
			BC--;
			{
				QT = 0;
				// keep SZC
				m_f.yx_val = ((A + TDAT8) << 4) | ((A + TDAT8) & 0x0f);
				m_f.h_val = m_f.n = 0;
				m_f.pv_val = !BC;
			}
			if (BC != 0) {
			if (nomreq_en)
				m_nomreq_cb(DE, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb006;
				return;
			}
			[[fallthrough]];
		case 0x06:
				PC -= 2;
				WZ = PC + 1;
				m_f.yx_val = PC >> 8;
			}
		}
		continue;

	case 0xb1:
		switch (u8(m_ref))
		{
		case 0x00:
		{
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedb101;
				} else {
					m_ref = 0xedb102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb103;
				return;
			}
			[[fallthrough]];
		case 0x03:
			WZ++;
			HL++;
			BC--;
			u8 res = A - TDAT8;
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = res;
				m_f.h_val = A ^ TDAT8 ^ res;
				if (m_f.h())
					res -= 1;
				m_f.yx_val = (res << 4) | (res & 0x0f);
				m_f.pv_val = !BC;
				m_f.n = 1;
			}
		}
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb104;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (BC != 0 && !m_f.z()) {
				PC -= 2;
				WZ = PC + 1;
				m_f.yx_val = PC >> 8;
			}
		}
		continue;

	case 0xb2:
		switch (u8(m_ref))
		{
		case 0x00:
			if (B == 1) {
				m_icount -= 1;
				if (m_icount <= 0) {
					m_ref = 0xedb201;
					return;
				}
				[[fallthrough]];
		case 0x01:
				;
			}
		{
			[[fallthrough]];
		case 0x02:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xedb202;
				} else {
					m_ref = 0xedb203;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			WZ = BC + 1;
			B--;
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedb204;
				} else {
					m_ref = 0xedb205;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			HL++;
			const u16 t = ((C + 1) & 0xff) + TDAT8;
			{
				QT = 0;
				m_f.s_val = m_f.z_val = B;
				m_f.yx_val = B;
				m_f.h_val = t >> 4;
				m_f.pv_val = (t & 0x07) ^ B;
				m_f.n = TDAT8 & SF;
				m_f.c = t & 0x100;
			}
		}
			if (B != 0) {
				PC -= 2;
				block_io_interrupted_flags();
			}
		}
		continue;

	case 0xb3:
		switch (u8(m_ref))
		{
		case 0x00:
			if (B == 1) {
				m_icount -= 1;
				if (m_icount <= 0) {
					m_ref = 0xedb301;
					return;
				}
				[[fallthrough]];
		case 0x01:
				;
			}
		{
			[[fallthrough]];
		case 0x02:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedb302;
				} else {
					m_ref = 0xedb303;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			B--;
			WZ = BC + 1;
			[[fallthrough]];
		case 0x04:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xedb304;
				} else {
					m_ref = 0xedb305;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			HL++;
			const u16 t = L + TDAT8;
			{
				QT = 0;
				m_f.s_val = m_f.z_val = B;
				m_f.yx_val = B;
				m_f.h_val = t >> 4;
				m_f.pv_val = (t & 0x07) ^ B;
				m_f.n = TDAT8 & SF;
				m_f.c = t & 0x100;
			}
		}
			if (B != 0) {
			if (nomreq_en)
				m_nomreq_cb(BC, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb306;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(BC, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb307;
				return;
			}
			[[fallthrough]];
		case 0x07:
			if (nomreq_en)
				m_nomreq_cb(BC, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb308;
				return;
			}
			[[fallthrough]];
		case 0x08:
			if (nomreq_en)
				m_nomreq_cb(BC, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb309;
				return;
			}
			[[fallthrough]];
		case 0x09:
			if (nomreq_en)
				m_nomreq_cb(BC, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb30a;
				return;
			}
			[[fallthrough]];
		case 0x0a:
				PC -= 2;
				block_io_interrupted_flags();
			}
		}
		continue;

	case 0xb8:
		switch (u8(m_ref))
		{
		case 0x00:
		{
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedb801;
				} else {
					m_ref = 0xedb802;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			[[fallthrough]];
		case 0x03:
			data_write(DE, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedb803;
				} else {
					m_ref = 0xedb804;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(DE, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb805;
				return;
			}
			[[fallthrough]];
		case 0x05:
			HL--;
			DE--;
			BC--;
			{
				QT = 0;
				// keep SZC
				m_f.yx_val = ((A + TDAT8) << 4) | ((A + TDAT8) & 0x0f);
				m_f.h_val = m_f.n = 0;
				m_f.pv_val = !BC;
			}
		}
			if (BC != 0) {
			if (nomreq_en)
				m_nomreq_cb(DE, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb806;
				return;
			}
			[[fallthrough]];
		case 0x06:
				PC -= 2;
				WZ = PC + 1;
				m_f.yx_val = PC >> 8;
			}
		}
		continue;

	case 0xb9:
		switch (u8(m_ref))
		{
		case 0x00:
		{
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedb901;
				} else {
					m_ref = 0xedb902;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(HL, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb903;
				return;
			}
			[[fallthrough]];
		case 0x03:
			WZ--;
			HL--;
			BC--;
			u8 res = A - TDAT8;
			{
				QT = 0;
				// keep C
				m_f.s_val = m_f.z_val = res;
				m_f.h_val = A ^ TDAT8 ^ res;
				if (m_f.h())
					res -= 1;
				m_f.yx_val = (res << 4) | (res & 0x0f);
				m_f.pv_val = !BC;
				m_f.n = 1;
			}
		}
			if (nomreq_en)
				m_nomreq_cb(HL /* suspected wrong. must be inside if? */, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb904;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (BC != 0 && !m_f.z()) {
				PC -= 2;
				WZ = PC + 1;
				m_f.yx_val = PC >> 8;
			}
		}
		continue;

	case 0xba:
		switch (u8(m_ref))
		{
		case 0x00:
			if (B == 1) {
				m_icount -= 1;
				if (m_icount <= 0) {
					m_ref = 0xedba01;
					return;
				}
				[[fallthrough]];
		case 0x01:
				;
			}
		{
			[[fallthrough]];
		case 0x02:
			TDAT8 = m_io.read_interruptible(BC);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xedba02;
				} else {
					m_ref = 0xedba03;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			WZ = BC - 1;
			B--;
			[[fallthrough]];
		case 0x04:
			data_write(HL, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedba04;
				} else {
					m_ref = 0xedba05;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			HL--;
			const u16 t = ((C - 1) & 0xff) + TDAT8;
			{
				QT = 0;
				m_f.s_val = m_f.z_val = B;
				m_f.yx_val = B;
				m_f.h_val = t >> 4;
				m_f.pv_val = (t & 0x07) ^ B;
				m_f.n = TDAT8 & SF;
				m_f.c = t & 0x100;
			}
		}
			if (B != 0) {
				PC -= 2;
				block_io_interrupted_flags();
			}
		}
		continue;

	case 0xbb:
		switch (u8(m_ref))
		{
		case 0x00:
			if (B == 1) {
				m_icount -= 1;
				if (m_icount <= 0) {
					m_ref = 0xedbb01;
					return;
				}
				[[fallthrough]];
		case 0x01:
				;
			}
		{
			[[fallthrough]];
		case 0x02:
			TDAT8 = data_read(HL);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xedbb02;
				} else {
					m_ref = 0xedbb03;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			B--;
			WZ = BC - 1;
			[[fallthrough]];
		case 0x04:
			m_io.write_interruptible(BC, TDAT8);
			m_icount -= m_iorq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_iorq_cycles;
					m_ref = 0xedbb04;
				} else {
					m_ref = 0xedbb05;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			HL--;
			const u16 t = L + TDAT8;
			{
				QT = 0;
				m_f.s_val = m_f.z_val = B;
				m_f.yx_val = B;
				m_f.h_val = t >> 4;
				m_f.pv_val = (t & 0x07) ^ B;
				m_f.n = TDAT8 & SF;
				m_f.c = t & 0x100;
			}
		}
			if (B != 0) {
				PC -= 2;
				block_io_interrupted_flags();
			}
		}
		continue;

	case 0xc1:
		switch (u8(m_ref))
		{
		case 0x00:
			mulub(B);
			m_icount -= 12;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0xc3:
		switch (u8(m_ref))
		{
		case 0x00:
			muluw(BC);
			m_icount -= 34;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0xc9:
		switch (u8(m_ref))
		{
		case 0x00:
			mulub(C);
			m_icount -= 12;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0xd1:
		switch (u8(m_ref))
		{
		case 0x00:
			mulub(D);
			m_icount -= 12;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0xd9:
		switch (u8(m_ref))
		{
		case 0x00:
			mulub(E);
			m_icount -= 12;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0xe1:
		switch (u8(m_ref))
		{
		case 0x00:
			mulub(H);
			m_icount -= 12;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0xe9:
		switch (u8(m_ref))
		{
		case 0x00:
			mulub(L);
			m_icount -= 12;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0xf1:
		switch (u8(m_ref))
		{
		case 0x00:
			mulub(A);
			m_icount -= 12;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0xf3:
		switch (u8(m_ref))
		{
		case 0x00:
			muluw(SP);
			m_icount -= 34;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	case 0xf9:
		switch (u8(m_ref))
		{
		case 0x00:
			mulub(A);
			m_icount -= 12;
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
		}
		continue;

	}
} break; // prefix: ed

case 0xfd:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x21:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2101;
				} else {
					m_ref = 0xfd2102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2103;
				} else {
					m_ref = 0xfd2104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			IY = TDAT;
		}
		continue;

	case 0x22:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2201;
				} else {
					m_ref = 0xfd2202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2203;
				} else {
					m_ref = 0xfd2204;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			TDAT = IY;
			[[fallthrough]];
		case 0x05:
			data_write(m_ea, TDAT_L);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2205;
				} else {
					m_ref = 0xfd2206;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			data_write(m_ea+1, TDAT_H);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2207;
				} else {
					m_ref = 0xfd2208;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			WZ = m_ea + 1;
		}
		continue;

	case 0x23:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd2301;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd2302;
				return;
			}
			[[fallthrough]];
		case 0x02:
			IY++;
		}
		continue;

	case 0x26:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2601;
				} else {
					m_ref = 0xfd2602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			HY = TDAT8;
		}
		continue;

	case 0x2a:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2a01;
				} else {
					m_ref = 0xfd2a02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2a03;
				} else {
					m_ref = 0xfd2a04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			m_ea = TDAT;
			[[fallthrough]];
		case 0x05:
			TDAT_L = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2a05;
				} else {
					m_ref = 0xfd2a06;
				}
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			TDAT_H = data_read(m_ea+1);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2a07;
				} else {
					m_ref = 0xfd2a08;
				}
				return;
			}
			[[fallthrough]];
		case 0x08:
			IY = TDAT;
			WZ = m_ea + 1;
		}
		continue;

	case 0x2b:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd2b01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd2b02;
				return;
			}
			[[fallthrough]];
		case 0x02:
			IY--;
		}
		continue;

	case 0x2e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd2e01;
				} else {
					m_ref = 0xfd2e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			LY = TDAT8;
		}
		continue;

	case 0x34:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd3401;
				} else {
					m_ref = 0xfd3402;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3403;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3404;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3405;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3406;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3407;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd3408;
				} else {
					m_ref = 0xfd3409;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd340a;
				return;
			}
			[[fallthrough]];
		case 0x0a:
			inc(TDAT8);
			[[fallthrough]];
		case 0x0b:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd340b;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x35:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd3501;
				} else {
					m_ref = 0xfd3502;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3503;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3504;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3505;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3506;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3507;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd3508;
				} else {
					m_ref = 0xfd3509;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd350a;
				return;
			}
			[[fallthrough]];
		case 0x0a:
			dec(TDAT8);
			[[fallthrough]];
		case 0x0b:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd350b;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x36:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd3601;
				} else {
					m_ref = 0xfd3602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			[[fallthrough]];
		case 0x03:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd3603;
				} else {
					m_ref = 0xfd3604;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd3606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			[[fallthrough]];
		case 0x07:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd3607;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x46:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd4601;
				} else {
					m_ref = 0xfd4602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd4603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd4604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd4605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd4606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd4607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd4608;
				} else {
					m_ref = 0xfd4609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			B = TDAT8;
		}
		continue;

	case 0x4e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd4e01;
				} else {
					m_ref = 0xfd4e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd4e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd4e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd4e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd4e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd4e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd4e08;
				} else {
					m_ref = 0xfd4e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			C = TDAT8;
		}
		continue;

	case 0x56:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd5601;
				} else {
					m_ref = 0xfd5602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd5603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd5604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd5605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd5606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd5607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd5608;
				} else {
					m_ref = 0xfd5609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			D = TDAT8;
		}
		continue;

	case 0x5e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd5e01;
				} else {
					m_ref = 0xfd5e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd5e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd5e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd5e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd5e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd5e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd5e08;
				} else {
					m_ref = 0xfd5e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			E = TDAT8;
		}
		continue;

	case 0x66:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd6601;
				} else {
					m_ref = 0xfd6602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd6603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd6604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd6605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd6606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd6607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd6608;
				} else {
					m_ref = 0xfd6609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			H = TDAT8;
		}
		continue;

	case 0x6e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd6e01;
				} else {
					m_ref = 0xfd6e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd6e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd6e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd6e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd6e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd6e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd6e08;
				} else {
					m_ref = 0xfd6e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			L = TDAT8;
		}
		continue;

	case 0x70:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7001;
				} else {
					m_ref = 0xfd7002;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7003;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7004;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7005;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7006;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7007;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = B;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7008;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x71:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7101;
				} else {
					m_ref = 0xfd7102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7103;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7104;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7105;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7106;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7107;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = C;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7108;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x72:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7201;
				} else {
					m_ref = 0xfd7202;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7203;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7204;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7205;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7206;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7207;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = D;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7208;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x73:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7301;
				} else {
					m_ref = 0xfd7302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7303;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7304;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7305;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7306;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7307;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = E;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7308;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x74:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7401;
				} else {
					m_ref = 0xfd7402;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7403;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7404;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7405;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7406;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7407;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = H;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7408;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x75:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7501;
				} else {
					m_ref = 0xfd7502;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7503;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7504;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7505;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7506;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7507;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = L;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7508;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x77:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7701;
				} else {
					m_ref = 0xfd7702;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7703;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7704;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7705;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7706;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7707;
				return;
			}
			[[fallthrough]];
		case 0x07:
			TDAT8 = A;
			[[fallthrough]];
		case 0x08:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7708;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x7e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7e01;
				} else {
					m_ref = 0xfd7e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd7e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd7e08;
				} else {
					m_ref = 0xfd7e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			A = TDAT8;
		}
		continue;

	case 0x86:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd8601;
				} else {
					m_ref = 0xfd8602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd8603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd8604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd8605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd8606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd8607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd8608;
				} else {
					m_ref = 0xfd8609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			add_a(TDAT8);
		}
		continue;

	case 0x8e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd8e01;
				} else {
					m_ref = 0xfd8e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd8e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd8e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd8e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd8e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd8e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd8e08;
				} else {
					m_ref = 0xfd8e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			adc_a(TDAT8);
		}
		continue;

	case 0x96:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd9601;
				} else {
					m_ref = 0xfd9602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd9603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd9604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd9605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd9606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd9607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd9608;
				} else {
					m_ref = 0xfd9609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			sub_a(TDAT8);
		}
		continue;

	case 0x9e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd9e01;
				} else {
					m_ref = 0xfd9e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd9e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd9e04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd9e05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd9e06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfd9e07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfd9e08;
				} else {
					m_ref = 0xfd9e09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			sbc_a(TDAT8);
		}
		continue;

	case 0xa6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfda601;
				} else {
					m_ref = 0xfda602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfda603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfda604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfda605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfda606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfda607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfda608;
				} else {
					m_ref = 0xfda609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			and_a(TDAT8);
		}
		continue;

	case 0xae:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfdae01;
				} else {
					m_ref = 0xfdae02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdae03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdae04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdae05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdae06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdae07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfdae08;
				} else {
					m_ref = 0xfdae09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			xor_a(TDAT8);
		}
		continue;

	case 0xb6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfdb601;
				} else {
					m_ref = 0xfdb602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdb603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdb604;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdb605;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdb606;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdb607;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfdb608;
				} else {
					m_ref = 0xfdb609;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			or_a(TDAT8);
		}
		continue;

	case 0xbe:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfdbe01;
				} else {
					m_ref = 0xfdbe02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdbe03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdbe04;
				return;
			}
			[[fallthrough]];
		case 0x04:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdbe05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdbe06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdbe07;
				return;
			}
			[[fallthrough]];
		case 0x07:
			[[fallthrough]];
		case 0x08:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfdbe08;
				} else {
					m_ref = 0xfdbe09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			cp(TDAT8);
		}
		continue;

	case 0xcb:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfdcb01;
				} else {
					m_ref = 0xfdcb02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			PC++;
			m_ea = (u16)(IY + (s8)TDAT8);
			WZ = m_ea;
			[[fallthrough]];
		case 0x03:
			TDAT8 = arg_read();
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfdcb03;
				} else {
					m_ref = 0xfdcb04;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			PC++;
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdcb05;
				return;
			}
			[[fallthrough]];
		case 0x05:
			if (nomreq_en)
				m_nomreq_cb(PC-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdcb06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			m_ref = (0xfe << 16) | (TDAT8 << 8);
			goto process;
		}
		continue;

	case 0xe1:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfde101;
				} else {
					m_ref = 0xfde102;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfde103;
				} else {
					m_ref = 0xfde104;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			IY = TDAT;
		}
		continue;

	case 0xe3:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT_L = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfde301;
				} else {
					m_ref = 0xfde302;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP++;
			[[fallthrough]];
		case 0x03:
			TDAT_H = stack_read(SP);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfde303;
				} else {
					m_ref = 0xfde304;
				}
				return;
			}
			[[fallthrough]];
		case 0x04:
			SP++;
			if (nomreq_en)
				m_nomreq_cb(SP-1, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfde305;
				return;
			}
			[[fallthrough]];
		case 0x05:
			SP--;
			[[fallthrough]];
		case 0x06:
			stack_write(SP, IY >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfde306;
				} else {
					m_ref = 0xfde307;
				}
				return;
			}
			[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, IY);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfde308;
				} else {
					m_ref = 0xfde309;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			IY = TDAT;
			if (nomreq_en)
				m_nomreq_cb(SP, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfde30a;
				return;
			}
			[[fallthrough]];
		case 0x0a:
			if (nomreq_en)
				m_nomreq_cb(SP, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfde30b;
				return;
			}
			[[fallthrough]];
		case 0x0b:
			WZ = TDAT;
		}
		continue;

	case 0xe5:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfde501;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, IY >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfde502;
				} else {
					m_ref = 0xfde503;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, IY);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfde504;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xf9:
		switch (u8(m_ref))
		{
		case 0x00:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdf901;
				return;
			}
			[[fallthrough]];
		case 0x01:
			if (nomreq_en)
				m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfdf902;
				return;
			}
			[[fallthrough]];
		case 0x02:
			SP = IY;
		}
		continue;

	}
} break; // prefix: fd

case 0xfe:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x06:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe0601;
				} else {
					m_ref = 0xfe0602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe0603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = rlc(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe0604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x0e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe0e01;
				} else {
					m_ref = 0xfe0e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe0e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = rrc(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe0e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x16:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe1601;
				} else {
					m_ref = 0xfe1602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe1603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = rl(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe1604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x1e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe1e01;
				} else {
					m_ref = 0xfe1e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe1e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = rr(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe1e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x26:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe2601;
				} else {
					m_ref = 0xfe2602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe2603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = sla(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe2604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x2e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe2e01;
				} else {
					m_ref = 0xfe2e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe2e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = sra(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe2e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x36:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe3601;
				} else {
					m_ref = 0xfe3602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe3603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = sll(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe3604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x3e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe3e01;
				} else {
					m_ref = 0xfe3e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe3e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = srl(TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe3e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x46:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe4601;
				} else {
					m_ref = 0xfe4602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe4603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_xy(0, TDAT8);
		}
		continue;

	case 0x4e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe4e01;
				} else {
					m_ref = 0xfe4e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe4e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_xy(1, TDAT8);
		}
		continue;

	case 0x56:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe5601;
				} else {
					m_ref = 0xfe5602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe5603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_xy(2, TDAT8);
		}
		continue;

	case 0x5e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe5e01;
				} else {
					m_ref = 0xfe5e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe5e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_xy(3, TDAT8);
		}
		continue;

	case 0x66:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe6601;
				} else {
					m_ref = 0xfe6602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe6603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_xy(4, TDAT8);
		}
		continue;

	case 0x6e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe6e01;
				} else {
					m_ref = 0xfe6e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe6e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_xy(5, TDAT8);
		}
		continue;

	case 0x76:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe7601;
				} else {
					m_ref = 0xfe7602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe7603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_xy(6, TDAT8);
		}
		continue;

	case 0x7e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe7e01;
				} else {
					m_ref = 0xfe7e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe7e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			bit_xy(7, TDAT8);
		}
		continue;

	case 0x86:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe8601;
				} else {
					m_ref = 0xfe8602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe8603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(0, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe8604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x8e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe8e01;
				} else {
					m_ref = 0xfe8e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe8e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(1, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe8e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x96:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe9601;
				} else {
					m_ref = 0xfe9602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe9603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(2, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe9604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0x9e:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe9e01;
				} else {
					m_ref = 0xfe9e02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfe9e03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(3, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfe9e04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xa6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfea601;
				} else {
					m_ref = 0xfea602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfea603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(4, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfea604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xae:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfeae01;
				} else {
					m_ref = 0xfeae02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfeae03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(5, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfeae04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xb6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfeb601;
				} else {
					m_ref = 0xfeb602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfeb603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(6, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfeb604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xbe:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfebe01;
				} else {
					m_ref = 0xfebe02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfebe03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = res(7, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfebe04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xc6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfec601;
				} else {
					m_ref = 0xfec602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfec603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(0, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfec604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xce:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfece01;
				} else {
					m_ref = 0xfece02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfece03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(1, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfece04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xd6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfed601;
				} else {
					m_ref = 0xfed602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfed603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(2, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfed604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xde:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfede01;
				} else {
					m_ref = 0xfede02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfede03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(3, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfede04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xe6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfee601;
				} else {
					m_ref = 0xfee602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfee603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(4, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfee604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xee:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfeee01;
				} else {
					m_ref = 0xfeee02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfeee03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(5, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfeee04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xf6:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfef601;
				} else {
					m_ref = 0xfef602;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfef603;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(6, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfef604;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	case 0xfe:
		switch (u8(m_ref))
		{
		case 0x00:
			[[fallthrough]];
		case 0x01:
			TDAT8 = data_read(m_ea);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfefe01;
				} else {
					m_ref = 0xfefe02;
				}
				return;
			}
			[[fallthrough]];
		case 0x02:
			if (nomreq_en)
				m_nomreq_cb(m_ea, 0x00, 0xff);
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xfefe03;
				return;
			}
			[[fallthrough]];
		case 0x03:
			TDAT8 = set(7, TDAT8);
			[[fallthrough]];
		case 0x04:
			data_write(m_ea, TDAT8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xfefe04;
				} else {
					m_ref = 0xffff00;
				}
				return;
			}
		}
		continue;

	}
} break; // prefix: fe

case 0xff:
{
		switch (u8(m_ref))
		{
		case 0x00:
			if (m_icount <= 0) {
				m_ref = 0xffff00;
				return;
			}
			if (m_service_attention) {
				if (m_busreq_state) { // SA_BUSREQ
					if (!m_busack_state) {
						m_busack_state = 1;
						m_busack_cb(1);
					}
					if (m_icount > 0)
						m_icount = 0;
					m_ref = 0xffff00;
					return;
				} else {
					set_service_attention<SA_BUSREQ, 0>();
					if (m_busack_state) {
						m_busack_state = 0;
						m_busack_cb(0);
					}
				}
			if (get_service_attention<SA_NMI_PENDING>()) {
			// Check if processor was halted
			leave_halt();
			if (HAS_LDAIR_QUIRK)
				// reset parity flag after LD A,I or LD A,R
				if (get_service_attention<SA_AFTER_LDAIR>()) m_f.pv_val = !0;
			m_iff1 = 0;
			m_r++;
			m_icount -= 5;
			if (m_icount <= 0) {
				m_ref = 0xffff01;
				return;
			}
			[[fallthrough]];
		case 0x01:
			SP--;
			[[fallthrough]];
		case 0x02:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff02;
				} else {
					m_ref = 0xffff03;
				}
				return;
			}
			[[fallthrough]];
		case 0x03:
			SP--;
			[[fallthrough]];
		case 0x04:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff04;
				} else {
					m_ref = 0xffff05;
				}
				return;
			}
			[[fallthrough]];
		case 0x05:
			PC = 0x0066;
			WZ = PC;
			set_service_attention<SA_NMI_PENDING, 0>();
			} else if (m_irq_state != CLEAR_LINE && m_iff1 && !get_service_attention<SA_AFTER_EI>()) { // SA_IRQ_ON
			// check if processor was halted
			leave_halt();
			// clear both interrupt flip flops
			m_iff1 = m_iff2 = 0;
			// say hi
			// Not precise in all cases. z80 must finish current instruction (NOP) to reach this state - in such case frame timings are shifter from cb event if calculated based on it.
			m_irqack_cb(1);
			m_r++;
		{
			// fetch the IRQ vector
			device_z80daisy_interface *intf = daisy_get_irq_device();
			if (intf != nullptr) {
				m_tmp_irq_vector = intf->z80daisy_irq_ack();
				if (debugger_enabled())
					device_execute_interface::device().debug()->interrupt_hook(0, m_pc.w);
			} else {
				m_tmp_irq_vector = standard_irq_callback(0, m_pc.w);
			}
			LOGMASKED(LOG_INT, "single INT m_tmp_irq_vector $%02x\n", m_tmp_irq_vector);
		}
			// 'interrupt latency' cycles
			m_icount -= 2;
			if (m_icount <= 0) {
				m_ref = 0xffff06;
				return;
			}
			[[fallthrough]];
		case 0x06:
			// Interrupt mode 2. Call [i:databyte]
			if (m_im == 2) {
				// Zilog's datasheet claims that "the least-significant bit must be a zero."
				// However, experiments have confirmed that IM 2 vectors do not have to be
				// even, and all 8 bits will be used; even $FF is handled normally.
				// CALL opcode timing
				m_icount -= 5;
				if (m_icount <= 0) {
					m_ref = 0xffff07;
					return;
				}
				[[fallthrough]];
		case 0x07:
			SP--;
			[[fallthrough]];
		case 0x08:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff08;
				} else {
					m_ref = 0xffff09;
				}
				return;
			}
			[[fallthrough]];
		case 0x09:
			SP--;
			[[fallthrough]];
		case 0x0a:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff0a;
				} else {
					m_ref = 0xffff0b;
				}
				return;
			}
			[[fallthrough]];
		case 0x0b:
				m_tmp_irq_vector = (m_tmp_irq_vector & 0xff) | (m_i << 8);
			[[fallthrough]];
		case 0x0c:
			TDAT_L = data_read(m_tmp_irq_vector);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff0c;
				} else {
					m_ref = 0xffff0d;
				}
				return;
			}
			[[fallthrough]];
		case 0x0d:
			[[fallthrough]];
		case 0x0e:
			TDAT_H = data_read(m_tmp_irq_vector+1);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff0e;
				} else {
					m_ref = 0xffff0f;
				}
				return;
			}
			[[fallthrough]];
		case 0x0f:
				PC = TDAT;
				LOGMASKED(LOG_INT, "IM2 [$%04x] = $%04x\n", m_tmp_irq_vector, PC);
			} else if (m_im == 1) {
				// Interrupt mode 1. RST 38h
				LOGMASKED(LOG_INT, "'%s' IM1 $0038\n", tag());
				// RST $38
				m_icount -= 5;
				if (m_icount <= 0) {
					m_ref = 0xffff10;
					return;
				}
				[[fallthrough]];
		case 0x10:
			SP--;
			[[fallthrough]];
		case 0x11:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff11;
				} else {
					m_ref = 0xffff12;
				}
				return;
			}
			[[fallthrough]];
		case 0x12:
			SP--;
			[[fallthrough]];
		case 0x13:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff13;
				} else {
					m_ref = 0xffff14;
				}
				return;
			}
			[[fallthrough]];
		case 0x14:
				PC = 0x0038;
			} else {
				/* Interrupt mode 0. We check for CALL and JP instructions,
				   if neither of these were found we assume a 1 byte opcode
				   was placed on the databus */
				LOGMASKED(LOG_INT, "IM0 $%04x\n", m_tmp_irq_vector);
				// check for nop
				if (m_tmp_irq_vector != 0x00) {
					if ((m_tmp_irq_vector & 0xff0000) == 0xcd0000) {
						// CALL $xxxx cycles
						m_icount -= 11;
						if (m_icount <= 0) {
							m_ref = 0xffff15;
							return;
						}
						[[fallthrough]];
		case 0x15:
			SP--;
			[[fallthrough]];
		case 0x16:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff16;
				} else {
					m_ref = 0xffff17;
				}
				return;
			}
			[[fallthrough]];
		case 0x17:
			SP--;
			[[fallthrough]];
		case 0x18:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff18;
				} else {
					m_ref = 0xffff19;
				}
				return;
			}
			[[fallthrough]];
		case 0x19:
						PC = m_tmp_irq_vector & 0xffff;
					} else if ((m_tmp_irq_vector & 0xff0000) == 0xc30000) {
						// JP $xxxx cycles
						m_icount -= 10;
						if (m_icount <= 0) {
							m_ref = 0xffff1a;
							return;
						}
						[[fallthrough]];
		case 0x1a:
						PC = m_tmp_irq_vector & 0xffff;
					} else if ((m_tmp_irq_vector & 0xc7) == 0xc7) {
						// RST $xx cycles
						m_icount -= 5;
						if (m_icount <= 0) {
							m_ref = 0xffff1b;
							return;
						}
						[[fallthrough]];
		case 0x1b:
			SP--;
			[[fallthrough]];
		case 0x1c:
			stack_write(SP, PC >> 8);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff1c;
				} else {
					m_ref = 0xffff1d;
				}
				return;
			}
			[[fallthrough]];
		case 0x1d:
			SP--;
			[[fallthrough]];
		case 0x1e:
			stack_write(SP, PC);
			m_icount -= m_mreq_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_mreq_cycles;
					m_ref = 0xffff1e;
				} else {
					m_ref = 0xffff1f;
				}
				return;
			}
			[[fallthrough]];
		case 0x1f:
						PC = m_tmp_irq_vector & 0x0038;
					} else if (m_tmp_irq_vector == 0xfb) {
						// EI cycles
						m_icount -= 4;
						if (m_icount <= 0) {
							m_ref = 0xffff20;
							return;
						}
						[[fallthrough]];
		case 0x20:
						ei();
					} else {
						logerror("take_interrupt: unexpected opcode in im0 mode: 0x%02x\n", m_tmp_irq_vector);
					}
				}
			}
			WZ = PC;
			if (HAS_LDAIR_QUIRK)
				// reset parity flag after LD A,I or LD A,R
				if (get_service_attention<SA_AFTER_LDAIR>()) m_f.pv_val = !0;
			}
				set_service_attention<SA_AFTER_EI, 0>();
				if (HAS_LDAIR_QUIRK)
					set_service_attention<SA_AFTER_LDAIR, 0>();
				if (m_halt) { // SA_HALT
					debugger_wait_hook();
			[[fallthrough]];
		case 0x21:
			TDAT8 = opcode_read();
			m_icount -= m_m1_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_m1_cycles;
					m_ref = 0xffff21;
				} else {
					m_ref = 0xffff22;
				}
				return;
			}
			[[fallthrough]];
		case 0x22:
			PC++;
			R++;
			Q = QT;
			QT = YXF;
					PC--;
					continue;
				}
			}
			PRVPC = PC;
			debugger_instruction_hook(PC);
			[[fallthrough]];
		case 0x23:
			TDAT8 = opcode_read();
			m_icount -= m_m1_cycles;
			if (m_icount <= 0) {
				if (access_to_be_redone()) {
					m_icount += m_m1_cycles;
					m_ref = 0xffff23;
				} else {
					m_ref = 0xffff24;
				}
				return;
			}
			[[fallthrough]];
		case 0x24:
			PC++;
			R++;
			Q = QT;
			QT = YXF;
			m_ref = (0x00 << 16) | (TDAT8 << 8);
			goto process;
		}
		continue;

} break; // prefix: ff

} // switch prefix

assert((void("switch statement above must cover all possible cases!"), false));
} // end: slow
if (m_ref != 0xffff00) goto process;

while (true) { // fast process
		// rop: unwrapped ff prefix
		//case 0x00:
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		if (m_service_attention) {
			if (m_busreq_state) { // SA_BUSREQ
				if (!m_busack_state) {
					m_busack_state = 1;
					m_busack_cb(1);
				}
				if (m_icount > 0)
					m_icount = 0;
				m_ref = 0xffff00;
				return;
			} else {
				set_service_attention<SA_BUSREQ, 0>();
				if (m_busack_state) {
					m_busack_state = 0;
					m_busack_cb(0);
				}
			}
		if (get_service_attention<SA_NMI_PENDING>()) {
		// Check if processor was halted
		leave_halt();
		if (HAS_LDAIR_QUIRK)
			// reset parity flag after LD A,I or LD A,R
			if (get_service_attention<SA_AFTER_LDAIR>()) m_f.pv_val = !0;
		m_iff1 = 0;
		m_r++;
		m_icount -= 5;
		if (m_icount <= 0) {
			m_ref = 0xffff01;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff02;
			} else {
				m_ref = 0xffff03;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff04;
			} else {
				m_ref = 0xffff05;
			}
			return;
		}
		//case 0x05:
		PC = 0x0066;
		WZ = PC;
		set_service_attention<SA_NMI_PENDING, 0>();
		} else if (m_irq_state != CLEAR_LINE && m_iff1 && !get_service_attention<SA_AFTER_EI>()) { // SA_IRQ_ON
		// check if processor was halted
		leave_halt();
		// clear both interrupt flip flops
		m_iff1 = m_iff2 = 0;
		// say hi
		// Not precise in all cases. z80 must finish current instruction (NOP) to reach this state - in such case frame timings are shifter from cb event if calculated based on it.
		m_irqack_cb(1);
		m_r++;
		{
		// fetch the IRQ vector
		device_z80daisy_interface *intf = daisy_get_irq_device();
		if (intf != nullptr) {
			m_tmp_irq_vector = intf->z80daisy_irq_ack();
			if (debugger_enabled())
				device_execute_interface::device().debug()->interrupt_hook(0, m_pc.w);
		} else {
			m_tmp_irq_vector = standard_irq_callback(0, m_pc.w);
		}
		LOGMASKED(LOG_INT, "single INT m_tmp_irq_vector $%02x\n", m_tmp_irq_vector);
		}
		// 'interrupt latency' cycles
		m_icount -= 2;
		if (m_icount <= 0) {
			m_ref = 0xffff06;
			return;
		}
		//case 0x06:
		// Interrupt mode 2. Call [i:databyte]
		if (m_im == 2) {
			// Zilog's datasheet claims that "the least-significant bit must be a zero."
			// However, experiments have confirmed that IM 2 vectors do not have to be
			// even, and all 8 bits will be used; even $FF is handled normally.
			// CALL opcode timing
			m_icount -= 5;
			if (m_icount <= 0) {
				m_ref = 0xffff07;
				return;
			}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff08;
			} else {
				m_ref = 0xffff09;
			}
			return;
		}
		//case 0x09:
		SP--;
		//case 0x0a:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff0a;
			} else {
				m_ref = 0xffff0b;
			}
			return;
		}
		//case 0x0b:
			m_tmp_irq_vector = (m_tmp_irq_vector & 0xff) | (m_i << 8);
		//case 0x0c:
		TDAT_L = data_read(m_tmp_irq_vector);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff0c;
			} else {
				m_ref = 0xffff0d;
			}
			return;
		}
		//case 0x0d:
		//case 0x0e:
		TDAT_H = data_read(m_tmp_irq_vector+1);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff0e;
			} else {
				m_ref = 0xffff0f;
			}
			return;
		}
		//case 0x0f:
			PC = TDAT;
			LOGMASKED(LOG_INT, "IM2 [$%04x] = $%04x\n", m_tmp_irq_vector, PC);
		} else if (m_im == 1) {
			// Interrupt mode 1. RST 38h
			LOGMASKED(LOG_INT, "'%s' IM1 $0038\n", tag());
			// RST $38
			m_icount -= 5;
			if (m_icount <= 0) {
				m_ref = 0xffff10;
				return;
			}
		//case 0x10:
		SP--;
		//case 0x11:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff11;
			} else {
				m_ref = 0xffff12;
			}
			return;
		}
		//case 0x12:
		SP--;
		//case 0x13:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff13;
			} else {
				m_ref = 0xffff14;
			}
			return;
		}
		//case 0x14:
			PC = 0x0038;
		} else {
			/* Interrupt mode 0. We check for CALL and JP instructions,
			   if neither of these were found we assume a 1 byte opcode
			   was placed on the databus */
			LOGMASKED(LOG_INT, "IM0 $%04x\n", m_tmp_irq_vector);
			// check for nop
			if (m_tmp_irq_vector != 0x00) {
				if ((m_tmp_irq_vector & 0xff0000) == 0xcd0000) {
					// CALL $xxxx cycles
					m_icount -= 11;
					if (m_icount <= 0) {
						m_ref = 0xffff15;
						return;
					}
		//case 0x15:
		SP--;
		//case 0x16:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff16;
			} else {
				m_ref = 0xffff17;
			}
			return;
		}
		//case 0x17:
		SP--;
		//case 0x18:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff18;
			} else {
				m_ref = 0xffff19;
			}
			return;
		}
		//case 0x19:
					PC = m_tmp_irq_vector & 0xffff;
				} else if ((m_tmp_irq_vector & 0xff0000) == 0xc30000) {
					// JP $xxxx cycles
					m_icount -= 10;
					if (m_icount <= 0) {
						m_ref = 0xffff1a;
						return;
					}
		//case 0x1a:
					PC = m_tmp_irq_vector & 0xffff;
				} else if ((m_tmp_irq_vector & 0xc7) == 0xc7) {
					// RST $xx cycles
					m_icount -= 5;
					if (m_icount <= 0) {
						m_ref = 0xffff1b;
						return;
					}
		//case 0x1b:
		SP--;
		//case 0x1c:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff1c;
			} else {
				m_ref = 0xffff1d;
			}
			return;
		}
		//case 0x1d:
		SP--;
		//case 0x1e:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xffff1e;
			} else {
				m_ref = 0xffff1f;
			}
			return;
		}
		//case 0x1f:
					PC = m_tmp_irq_vector & 0x0038;
				} else if (m_tmp_irq_vector == 0xfb) {
					// EI cycles
					m_icount -= 4;
					if (m_icount <= 0) {
						m_ref = 0xffff20;
						return;
					}
		//case 0x20:
					ei();
				} else {
					logerror("take_interrupt: unexpected opcode in im0 mode: 0x%02x\n", m_tmp_irq_vector);
				}
			}
		}
		WZ = PC;
		if (HAS_LDAIR_QUIRK)
			// reset parity flag after LD A,I or LD A,R
			if (get_service_attention<SA_AFTER_LDAIR>()) m_f.pv_val = !0;
		}
			set_service_attention<SA_AFTER_EI, 0>();
			if (HAS_LDAIR_QUIRK)
				set_service_attention<SA_AFTER_LDAIR, 0>();
			if (m_halt) { // SA_HALT
				debugger_wait_hook();
		//case 0x21:
		TDAT8 = opcode_read();
		m_icount -= m_m1_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_m1_cycles;
				m_ref = 0xffff21;
			} else {
				m_ref = 0xffff22;
			}
			return;
		}
		//case 0x22:
		PC++;
		R++;
		Q = QT;
		QT = YXF;
				PC--;
				continue;
			}
		}
		PRVPC = PC;
		debugger_instruction_hook(PC);
		//case 0x23:
		TDAT8 = opcode_read();
		m_icount -= m_m1_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_m1_cycles;
				m_ref = 0xffff23;
			} else {
				m_ref = 0xffff24;
			}
			return;
		}
		//case 0x24:
		PC++;
		R++;
		Q = QT;
		QT = YXF;
		m_ref = (0x00 << 16) | (TDAT8 << 8);
		goto process;
		continue;


process:
switch (u8(m_ref >> 16)) // prefix
{
case 0x00:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x00:
		continue;

	case 0x01:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x000101;
			} else {
				m_ref = 0x000102;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x000103;
			} else {
				m_ref = 0x000104;
			}
			return;
		}
		//case 0x04:
		PC++;
		BC = TDAT;
		continue;

	case 0x02:
		//case 0x00:
		TDAT8 = A;
		//case 0x01:
		data_write(BC, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x000201;
			} else {
				m_ref = 0x000202;
			}
			return;
		}
		//case 0x02:
		WZ_L = (BC + 1) & 0xFF;
		WZ_H = A;
		continue;

	case 0x03:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x000301;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x000302;
			return;
		}
		//case 0x02:
		BC++;
		continue;

	case 0x04:
		inc(B);
		continue;

	case 0x05:
		dec(B);
		continue;

	case 0x06:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x000601;
			} else {
				m_ref = 0x000602;
			}
			return;
		}
		//case 0x02:
		PC++;
		B = TDAT8;
		continue;

	case 0x07:
		rlca();
		continue;

	case 0x08:
		F = get_f();
		using std::swap;
		swap(m_af, m_af2);
		set_f(F);
		continue;

	case 0x09:
		{
		const u32 res = HL +  BC;
		WZ = HL + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^  BC) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x0a:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(BC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x000a01;
			} else {
				m_ref = 0x000a02;
			}
			return;
		}
		//case 0x02:
		A = TDAT8;
		WZ = BC+1;
		continue;

	case 0x0b:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x000b01;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x000b02;
			return;
		}
		//case 0x02:
		BC--;
		continue;

	case 0x0c:
		inc(C);
		continue;

	case 0x0d:
		dec(C);
		continue;

	case 0x0e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x000e01;
			} else {
				m_ref = 0x000e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		C = TDAT8;
		continue;

	case 0x0f:
		rrca();
		continue;

	case 0x10:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x001001;
			return;
		}
		//case 0x01:
		if (--B) {
		//case 0x02:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x001002;
			} else {
				m_ref = 0x001003;
			}
			return;
		}
		//case 0x03:
		PC++;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x001004;
			return;
		}
		//case 0x04:
		PC += (s8)TDAT8;
		WZ = PC;
		} else {
		//case 0x05:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x001005;
			} else {
				m_ref = 0x001006;
			}
			return;
		}
		//case 0x06:
		PC++;
		}
		continue;

	case 0x11:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x001101;
			} else {
				m_ref = 0x001102;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x001103;
			} else {
				m_ref = 0x001104;
			}
			return;
		}
		//case 0x04:
		PC++;
		DE = TDAT;
		continue;

	case 0x12:
		//case 0x00:
		TDAT8 = A;
		//case 0x01:
		data_write(DE, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x001201;
			} else {
				m_ref = 0x001202;
			}
			return;
		}
		//case 0x02:
		WZ_L = (DE + 1) & 0xFF;
		WZ_H = A;
		continue;

	case 0x13:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x001301;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x001302;
			return;
		}
		//case 0x02:
		DE++;
		continue;

	case 0x14:
		inc(D);
		continue;

	case 0x15:
		dec(D);
		continue;

	case 0x16:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x001601;
			} else {
				m_ref = 0x001602;
			}
			return;
		}
		//case 0x02:
		PC++;
		D = TDAT8;
		continue;

	case 0x17:
		rla();
		continue;

	case 0x18:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x001801;
			} else {
				m_ref = 0x001802;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x001803;
			return;
		}
		//case 0x03:
		PC += (s8)TDAT8;
		WZ = PC;
		continue;

	case 0x19:
		{
		const u32 res = HL +  DE;
		WZ = HL + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^  DE) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x1a:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(DE);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x001a01;
			} else {
				m_ref = 0x001a02;
			}
			return;
		}
		//case 0x02:
		A = TDAT8;
		WZ = DE + 1;
		continue;

	case 0x1b:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x001b01;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x001b02;
			return;
		}
		//case 0x02:
		DE--;
		continue;

	case 0x1c:
		inc(E);
		continue;

	case 0x1d:
		dec(E);
		continue;

	case 0x1e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x001e01;
			} else {
				m_ref = 0x001e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		E = TDAT8;
		continue;

	case 0x1f:
		rra();
		continue;

	case 0x20:
		//case 0x00:
		if (!m_f.z()) {
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002001;
			} else {
				m_ref = 0x002002;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x002003;
			return;
		}
		//case 0x03:
		PC += (s8)TDAT8;
		WZ = PC;
		} else {
		//case 0x04:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002004;
			} else {
				m_ref = 0x002005;
			}
			return;
		}
		//case 0x05:
		PC++;
		}
		continue;

	case 0x21:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002101;
			} else {
				m_ref = 0x002102;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002103;
			} else {
				m_ref = 0x002104;
			}
			return;
		}
		//case 0x04:
		PC++;
		HL = TDAT;
		continue;

	case 0x22:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002201;
			} else {
				m_ref = 0x002202;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002203;
			} else {
				m_ref = 0x002204;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		TDAT = HL;
		//case 0x05:
		data_write(m_ea, TDAT_L);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002205;
			} else {
				m_ref = 0x002206;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		data_write(m_ea+1, TDAT_H);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002207;
			} else {
				m_ref = 0x002208;
			}
			return;
		}
		//case 0x08:
		WZ = m_ea + 1;
		continue;

	case 0x23:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x002301;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x002302;
			return;
		}
		//case 0x02:
		HL++;
		continue;

	case 0x24:
		inc(H);
		continue;

	case 0x25:
		dec(H);
		continue;

	case 0x26:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002601;
			} else {
				m_ref = 0x002602;
			}
			return;
		}
		//case 0x02:
		PC++;
		H = TDAT8;
		continue;

	case 0x27:
		daa();
		continue;

	case 0x28:
		//case 0x00:
		if (m_f.z()) {
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002801;
			} else {
				m_ref = 0x002802;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x002803;
			return;
		}
		//case 0x03:
		PC += (s8)TDAT8;
		WZ = PC;
		} else {
		//case 0x04:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002804;
			} else {
				m_ref = 0x002805;
			}
			return;
		}
		//case 0x05:
		PC++;
		}
		continue;

	case 0x29:
		{
		const u32 res = HL +  HL;
		WZ = HL + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^  HL) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x2a:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002a01;
			} else {
				m_ref = 0x002a02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002a03;
			} else {
				m_ref = 0x002a04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		//case 0x05:
		TDAT_L = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002a05;
			} else {
				m_ref = 0x002a06;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		TDAT_H = data_read(m_ea+1);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002a07;
			} else {
				m_ref = 0x002a08;
			}
			return;
		}
		//case 0x08:
		HL = TDAT;
		WZ = m_ea + 1;
		continue;

	case 0x2b:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x002b01;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x002b02;
			return;
		}
		//case 0x02:
		HL--;
		continue;

	case 0x2c:
		inc(L);
		continue;

	case 0x2d:
		dec(L);
		continue;

	case 0x2e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x002e01;
			} else {
				m_ref = 0x002e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		L = TDAT8;
		continue;

	case 0x2f:
		A ^= 0xff;
		{
			// keep SZPC
			m_f.yx_val = A;
			m_f.h_val = HF;
			m_f.n = 1;
		}
		continue;

	case 0x30:
		//case 0x00:
		if (!m_f.c) {
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003001;
			} else {
				m_ref = 0x003002;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x003003;
			return;
		}
		//case 0x03:
		PC += (s8)TDAT8;
		WZ = PC;
		} else {
		//case 0x04:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003004;
			} else {
				m_ref = 0x003005;
			}
			return;
		}
		//case 0x05:
		PC++;
		}
		continue;

	case 0x31:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003101;
			} else {
				m_ref = 0x003102;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003103;
			} else {
				m_ref = 0x003104;
			}
			return;
		}
		//case 0x04:
		PC++;
		SP = TDAT;
		continue;

	case 0x32:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003201;
			} else {
				m_ref = 0x003202;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003203;
			} else {
				m_ref = 0x003204;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		TDAT8 = A;
		//case 0x05:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003205;
			} else {
				m_ref = 0x003206;
			}
			return;
		}
		//case 0x06:
		WZ_L = (m_ea + 1) & 0xFF;
		WZ_H = A;
		continue;

	case 0x33:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x003301;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x003302;
			return;
		}
		//case 0x02:
		SP++;
		continue;

	case 0x34:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003401;
			} else {
				m_ref = 0x003402;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x003403;
			return;
		}
		//case 0x03:
		inc(TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003404;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x35:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003501;
			} else {
				m_ref = 0x003502;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x003503;
			return;
		}
		//case 0x03:
		dec(TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003504;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x36:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003601;
			} else {
				m_ref = 0x003602;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003603;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x37:
		{
		QT = 0;
		// keep SZP
		m_f.yx_val = (m_f.yx_val & Q) | A;
		m_f.h_val = m_f.n = 0;
		m_f.c = 1;
		}
		continue;

	case 0x38:
		//case 0x00:
		if (m_f.c) {
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003801;
			} else {
				m_ref = 0x003802;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x003803;
			return;
		}
		//case 0x03:
		PC += (s8)TDAT8;
		WZ = PC;
		} else {
		//case 0x04:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003804;
			} else {
				m_ref = 0x003805;
			}
			return;
		}
		//case 0x05:
		PC++;
		}
		continue;

	case 0x39:
		{
		const u32 res = HL +  SP;
		WZ = HL + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^  SP) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x3a:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003a01;
			} else {
				m_ref = 0x003a02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003a03;
			} else {
				m_ref = 0x003a04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		//case 0x05:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003a05;
			} else {
				m_ref = 0x003a06;
			}
			return;
		}
		//case 0x06:
		A = TDAT8;
		WZ = m_ea + 1;
		continue;

	case 0x3b:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x003b01;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x003b02;
			return;
		}
		//case 0x02:
		SP--;
		continue;

	case 0x3c:
		inc(A);
		continue;

	case 0x3d:
		dec(A);
		continue;

	case 0x3e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x003e01;
			} else {
				m_ref = 0x003e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		A = TDAT8;
		continue;

	case 0x3f:
		{
		QT = 0;
		// keep SZP
		m_f.yx_val = (m_f.yx_val & Q) | A;
		m_f.h_val = m_f.c << 4;
		m_f.n = 0;
		m_f.c = !m_f.c;
		}
		continue;

	case 0x40:
		continue;

	case 0x41:
		B = C;
		continue;

	case 0x42:
		B = D;
		continue;

	case 0x43:
		B = E;
		continue;

	case 0x44:
		B = H;
		continue;

	case 0x45:
		B = L;
		continue;

	case 0x46:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x004601;
			} else {
				m_ref = 0x004602;
			}
			return;
		}
		//case 0x02:
		B = TDAT8;
		continue;

	case 0x47:
		B = A;
		continue;

	case 0x48:
		C = B;
		continue;

	case 0x49:
		continue;

	case 0x4a:
		C = D;
		continue;

	case 0x4b:
		C = E;
		continue;

	case 0x4c:
		C = H;
		continue;

	case 0x4d:
		C = L;
		continue;

	case 0x4e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x004e01;
			} else {
				m_ref = 0x004e02;
			}
			return;
		}
		//case 0x02:
		C = TDAT8;
		continue;

	case 0x4f:
		C = A;
		continue;

	case 0x50:
		D = B;
		continue;

	case 0x51:
		D = C;
		continue;

	case 0x52:
		continue;

	case 0x53:
		D = E;
		continue;

	case 0x54:
		D = H;
		continue;

	case 0x55:
		D = L;
		continue;

	case 0x56:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x005601;
			} else {
				m_ref = 0x005602;
			}
			return;
		}
		//case 0x02:
		D = TDAT8;
		continue;

	case 0x57:
		D = A;
		continue;

	case 0x58:
		E = B;
		continue;

	case 0x59:
		E = C;
		continue;

	case 0x5a:
		E = D;
		continue;

	case 0x5b:
		continue;

	case 0x5c:
		E = H;
		continue;

	case 0x5d:
		E = L;
		continue;

	case 0x5e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x005e01;
			} else {
				m_ref = 0x005e02;
			}
			return;
		}
		//case 0x02:
		E = TDAT8;
		continue;

	case 0x5f:
		E = A;
		continue;

	case 0x60:
		H = B;
		continue;

	case 0x61:
		H = C;
		continue;

	case 0x62:
		H = D;
		continue;

	case 0x63:
		H = E;
		continue;

	case 0x64:
		continue;

	case 0x65:
		H = L;
		continue;

	case 0x66:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x006601;
			} else {
				m_ref = 0x006602;
			}
			return;
		}
		//case 0x02:
		H = TDAT8;
		continue;

	case 0x67:
		H = A;
		continue;

	case 0x68:
		L = B;
		continue;

	case 0x69:
		L = C;
		continue;

	case 0x6a:
		L = D;
		continue;

	case 0x6b:
		L = E;
		continue;

	case 0x6c:
		L = H;
		continue;

	case 0x6d:
		continue;

	case 0x6e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x006e01;
			} else {
				m_ref = 0x006e02;
			}
			return;
		}
		//case 0x02:
		L = TDAT8;
		continue;

	case 0x6f:
		L = A;
		continue;

	case 0x70:
		//case 0x00:
		TDAT = B;
		//case 0x01:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x007001;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x71:
		//case 0x00:
		TDAT = C;
		//case 0x01:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x007101;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x72:
		//case 0x00:
		TDAT = D;
		//case 0x01:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x007201;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x73:
		//case 0x00:
		TDAT = E;
		//case 0x01:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x007301;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x74:
		//case 0x00:
		TDAT = H;
		//case 0x01:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x007401;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x75:
		//case 0x00:
		TDAT = L;
		//case 0x01:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x007501;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x76:
		halt();
		continue;

	case 0x77:
		//case 0x00:
		TDAT = A;
		//case 0x01:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x007701;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x78:
		A = B;
		continue;

	case 0x79:
		A = C;
		continue;

	case 0x7a:
		A = D;
		continue;

	case 0x7b:
		A = E;
		continue;

	case 0x7c:
		A = H;
		continue;

	case 0x7d:
		A = L;
		continue;

	case 0x7e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x007e01;
			} else {
				m_ref = 0x007e02;
			}
			return;
		}
		//case 0x02:
		A = TDAT8;
		continue;

	case 0x7f:
		continue;

	case 0x80:
		add_a(B);
		continue;

	case 0x81:
		add_a(C);
		continue;

	case 0x82:
		add_a(D);
		continue;

	case 0x83:
		add_a(E);
		continue;

	case 0x84:
		add_a(H);
		continue;

	case 0x85:
		add_a(L);
		continue;

	case 0x86:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x008601;
			} else {
				m_ref = 0x008602;
			}
			return;
		}
		//case 0x02:
		add_a(TDAT8);
		continue;

	case 0x87:
		add_a(A);
		continue;

	case 0x88:
		adc_a(B);
		continue;

	case 0x89:
		adc_a(C);
		continue;

	case 0x8a:
		adc_a(D);
		continue;

	case 0x8b:
		adc_a(E);
		continue;

	case 0x8c:
		adc_a(H);
		continue;

	case 0x8d:
		adc_a(L);
		continue;

	case 0x8e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x008e01;
			} else {
				m_ref = 0x008e02;
			}
			return;
		}
		//case 0x02:
		adc_a(TDAT8);
		continue;

	case 0x8f:
		adc_a(A);
		continue;

	case 0x90:
		sub_a(B);
		continue;

	case 0x91:
		sub_a(C);
		continue;

	case 0x92:
		sub_a(D);
		continue;

	case 0x93:
		sub_a(E);
		continue;

	case 0x94:
		sub_a(H);
		continue;

	case 0x95:
		sub_a(L);
		continue;

	case 0x96:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x009601;
			} else {
				m_ref = 0x009602;
			}
			return;
		}
		//case 0x02:
		sub_a(TDAT8);
		continue;

	case 0x97:
		sub_a(A);
		continue;

	case 0x98:
		sbc_a(B);
		continue;

	case 0x99:
		sbc_a(C);
		continue;

	case 0x9a:
		sbc_a(D);
		continue;

	case 0x9b:
		sbc_a(E);
		continue;

	case 0x9c:
		sbc_a(H);
		continue;

	case 0x9d:
		sbc_a(L);
		continue;

	case 0x9e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x009e01;
			} else {
				m_ref = 0x009e02;
			}
			return;
		}
		//case 0x02:
		sbc_a(TDAT8);
		continue;

	case 0x9f:
		sbc_a(A);
		continue;

	case 0xa0:
		and_a(B);
		continue;

	case 0xa1:
		and_a(C);
		continue;

	case 0xa2:
		and_a(D);
		continue;

	case 0xa3:
		and_a(E);
		continue;

	case 0xa4:
		and_a(H);
		continue;

	case 0xa5:
		and_a(L);
		continue;

	case 0xa6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00a601;
			} else {
				m_ref = 0x00a602;
			}
			return;
		}
		//case 0x02:
		and_a(TDAT8);
		continue;

	case 0xa7:
		and_a(A);
		continue;

	case 0xa8:
		xor_a(B);
		continue;

	case 0xa9:
		xor_a(C);
		continue;

	case 0xaa:
		xor_a(D);
		continue;

	case 0xab:
		xor_a(E);
		continue;

	case 0xac:
		xor_a(H);
		continue;

	case 0xad:
		xor_a(L);
		continue;

	case 0xae:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ae01;
			} else {
				m_ref = 0x00ae02;
			}
			return;
		}
		//case 0x02:
		xor_a(TDAT8);
		continue;

	case 0xaf:
		xor_a(A);
		continue;

	case 0xb0:
		or_a(B);
		continue;

	case 0xb1:
		or_a(C);
		continue;

	case 0xb2:
		or_a(D);
		continue;

	case 0xb3:
		or_a(E);
		continue;

	case 0xb4:
		or_a(H);
		continue;

	case 0xb5:
		or_a(L);
		continue;

	case 0xb6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00b601;
			} else {
				m_ref = 0x00b602;
			}
			return;
		}
		//case 0x02:
		or_a(TDAT8);
		continue;

	case 0xb7:
		or_a(A);
		continue;

	case 0xb8:
		cp(B);
		continue;

	case 0xb9:
		cp(C);
		continue;

	case 0xba:
		cp(D);
		continue;

	case 0xbb:
		cp(E);
		continue;

	case 0xbc:
		cp(H);
		continue;

	case 0xbd:
		cp(L);
		continue;

	case 0xbe:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00be01;
			} else {
				m_ref = 0x00be02;
			}
			return;
		}
		//case 0x02:
		cp(TDAT8);
		continue;

	case 0xbf:
		cp(A);
		continue;

	case 0xc0:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00c001;
			return;
		}
		//case 0x01:
		if (!m_f.z()) {
		//case 0x02:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c002;
			} else {
				m_ref = 0x00c003;
			}
			return;
		}
		//case 0x03:
		SP++;
		//case 0x04:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c004;
			} else {
				m_ref = 0x00c005;
			}
			return;
		}
		//case 0x05:
		SP++;
			PC = TDAT;
			WZ = PC;
		}
		continue;

	case 0xc1:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c101;
			} else {
				m_ref = 0x00c102;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c103;
			} else {
				m_ref = 0x00c104;
			}
			return;
		}
		//case 0x04:
		SP++;
		BC = TDAT;
		continue;

	case 0xc2:
		//case 0x00:
		if (!m_f.z()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c201;
			} else {
				m_ref = 0x00c202;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c203;
			} else {
				m_ref = 0x00c204;
			}
			return;
		}
		//case 0x04:
		PC++;
		PC = TDAT;
			WZ = PC;
		} else {
			// implicit do PC += 2
		//case 0x05:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c205;
			} else {
				m_ref = 0x00c206;
			}
			return;
		}
		//case 0x06:
		PC++;
		//case 0x07:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c207;
			} else {
				m_ref = 0x00c208;
			}
			return;
		}
		//case 0x08:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xc3:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c301;
			} else {
				m_ref = 0x00c302;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c303;
			} else {
				m_ref = 0x00c304;
			}
			return;
		}
		//case 0x04:
		PC++;
		PC = TDAT;
		WZ = PC;
		continue;

	case 0xc4:
		//case 0x00:
		if (!m_f.z()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c401;
			} else {
				m_ref = 0x00c402;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c403;
			} else {
				m_ref = 0x00c404;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00c405;
			return;
		}
		//case 0x05:
		WZ = m_ea;
		SP--;
		//case 0x06:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c406;
			} else {
				m_ref = 0x00c407;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c408;
			} else {
				m_ref = 0x00c409;
			}
			return;
		}
		//case 0x09:
		PC = m_ea;
		} else {
		//case 0x0a:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c40a;
			} else {
				m_ref = 0x00c40b;
			}
			return;
		}
		//case 0x0b:
		PC++;
		//case 0x0c:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c40c;
			} else {
				m_ref = 0x00c40d;
			}
			return;
		}
		//case 0x0d:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xc5:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00c501;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, BC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c502;
			} else {
				m_ref = 0x00c503;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, BC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c504;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xc6:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c601;
			} else {
				m_ref = 0x00c602;
			}
			return;
		}
		//case 0x02:
		PC++;
		add_a(TDAT8);
		continue;

	case 0xc7:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00c701;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c702;
			} else {
				m_ref = 0x00c703;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c704;
			} else {
				m_ref = 0x00c705;
			}
			return;
		}
		//case 0x05:
		PC = 0x00;
		WZ = PC;
		continue;

	case 0xc8:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00c801;
			return;
		}
		//case 0x01:
		if (m_f.z()) {
		//case 0x02:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c802;
			} else {
				m_ref = 0x00c803;
			}
			return;
		}
		//case 0x03:
		SP++;
		//case 0x04:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c804;
			} else {
				m_ref = 0x00c805;
			}
			return;
		}
		//case 0x05:
		SP++;
			PC = TDAT;
			WZ = PC;
		}
		continue;

	case 0xc9:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c901;
			} else {
				m_ref = 0x00c902;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00c903;
			} else {
				m_ref = 0x00c904;
			}
			return;
		}
		//case 0x04:
		SP++;
		PC = TDAT;
		WZ = PC;
		continue;

	case 0xca:
		//case 0x00:
		if (m_f.z()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ca01;
			} else {
				m_ref = 0x00ca02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ca03;
			} else {
				m_ref = 0x00ca04;
			}
			return;
		}
		//case 0x04:
		PC++;
		PC = TDAT;
			WZ = PC;
		} else {
			// implicit do PC += 2
		//case 0x05:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ca05;
			} else {
				m_ref = 0x00ca06;
			}
			return;
		}
		//case 0x06:
		PC++;
		//case 0x07:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ca07;
			} else {
				m_ref = 0x00ca08;
			}
			return;
		}
		//case 0x08:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xcb:
		//case 0x00:
		//case 0x01:
		TDAT8 = opcode_read();
		m_icount -= m_m1_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_m1_cycles;
				m_ref = 0x00cb01;
			} else {
				m_ref = 0x00cb02;
			}
			return;
		}
		//case 0x02:
		PC++;
		R++;
		Q = QT;
		QT = YXF;
		m_ref = (0xcb << 16) | (TDAT8 << 8);
		goto process;
		continue;

	case 0xcc:
		//case 0x00:
		if (m_f.z()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cc01;
			} else {
				m_ref = 0x00cc02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cc03;
			} else {
				m_ref = 0x00cc04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00cc05;
			return;
		}
		//case 0x05:
		WZ = m_ea;
		SP--;
		//case 0x06:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cc06;
			} else {
				m_ref = 0x00cc07;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cc08;
			} else {
				m_ref = 0x00cc09;
			}
			return;
		}
		//case 0x09:
		PC = m_ea;
		} else {
		//case 0x0a:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cc0a;
			} else {
				m_ref = 0x00cc0b;
			}
			return;
		}
		//case 0x0b:
		PC++;
		//case 0x0c:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cc0c;
			} else {
				m_ref = 0x00cc0d;
			}
			return;
		}
		//case 0x0d:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xcd:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cd01;
			} else {
				m_ref = 0x00cd02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cd03;
			} else {
				m_ref = 0x00cd04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00cd05;
			return;
		}
		//case 0x05:
		WZ = m_ea;
		SP--;
		//case 0x06:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cd06;
			} else {
				m_ref = 0x00cd07;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cd08;
			} else {
				m_ref = 0x00cd09;
			}
			return;
		}
		//case 0x09:
		PC = m_ea;
		continue;

	case 0xce:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ce01;
			} else {
				m_ref = 0x00ce02;
			}
			return;
		}
		//case 0x02:
		PC++;
		adc_a(TDAT8);
		continue;

	case 0xcf:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00cf01;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cf02;
			} else {
				m_ref = 0x00cf03;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00cf04;
			} else {
				m_ref = 0x00cf05;
			}
			return;
		}
		//case 0x05:
		PC = 0x08;
		WZ = PC;
		continue;

	case 0xd0:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00d001;
			return;
		}
		//case 0x01:
		if (!m_f.c) {
		//case 0x02:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d002;
			} else {
				m_ref = 0x00d003;
			}
			return;
		}
		//case 0x03:
		SP++;
		//case 0x04:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d004;
			} else {
				m_ref = 0x00d005;
			}
			return;
		}
		//case 0x05:
		SP++;
			PC = TDAT;
			WZ = PC;
		}
		continue;

	case 0xd1:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d101;
			} else {
				m_ref = 0x00d102;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d103;
			} else {
				m_ref = 0x00d104;
			}
			return;
		}
		//case 0x04:
		SP++;
		DE = TDAT;
		continue;

	case 0xd2:
		//case 0x00:
		if (!m_f.c) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d201;
			} else {
				m_ref = 0x00d202;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d203;
			} else {
				m_ref = 0x00d204;
			}
			return;
		}
		//case 0x04:
		PC++;
		PC = TDAT;
			WZ = PC;
		} else {
			// implicit do PC += 2
		//case 0x05:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d205;
			} else {
				m_ref = 0x00d206;
			}
			return;
		}
		//case 0x06:
		PC++;
		//case 0x07:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d207;
			} else {
				m_ref = 0x00d208;
			}
			return;
		}
		//case 0x08:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xd3:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d301;
			} else {
				m_ref = 0x00d302;
			}
			return;
		}
		//case 0x02:
		PC++;
		TDAT2 = TDAT8 | (A << 8);
		TDAT = A;
		//case 0x03:
		m_io.write_interruptible(TDAT2, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0x00d303;
			} else {
				m_ref = 0x00d304;
			}
			return;
		}
		//case 0x04:
		WZ_L = ((TDAT2 & 0xff) + 1) & 0xff;
		WZ_H = A;
		continue;

	case 0xd4:
		//case 0x00:
		if (!m_f.c) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d401;
			} else {
				m_ref = 0x00d402;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d403;
			} else {
				m_ref = 0x00d404;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00d405;
			return;
		}
		//case 0x05:
		WZ = m_ea;
		SP--;
		//case 0x06:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d406;
			} else {
				m_ref = 0x00d407;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d408;
			} else {
				m_ref = 0x00d409;
			}
			return;
		}
		//case 0x09:
		PC = m_ea;
		} else {
		//case 0x0a:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d40a;
			} else {
				m_ref = 0x00d40b;
			}
			return;
		}
		//case 0x0b:
		PC++;
		//case 0x0c:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d40c;
			} else {
				m_ref = 0x00d40d;
			}
			return;
		}
		//case 0x0d:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xd5:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00d501;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, DE >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d502;
			} else {
				m_ref = 0x00d503;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, DE);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d504;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xd6:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d601;
			} else {
				m_ref = 0x00d602;
			}
			return;
		}
		//case 0x02:
		PC++;
		sub_a(TDAT8);
		continue;

	case 0xd7:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00d701;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d702;
			} else {
				m_ref = 0x00d703;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d704;
			} else {
				m_ref = 0x00d705;
			}
			return;
		}
		//case 0x05:
		PC = 0x10;
		WZ = PC;
		continue;

	case 0xd8:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00d801;
			return;
		}
		//case 0x01:
		if (m_f.c) {
		//case 0x02:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d802;
			} else {
				m_ref = 0x00d803;
			}
			return;
		}
		//case 0x03:
		SP++;
		//case 0x04:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00d804;
			} else {
				m_ref = 0x00d805;
			}
			return;
		}
		//case 0x05:
		SP++;
			PC = TDAT;
			WZ = PC;
		}
		continue;

	case 0xd9:
		exx();
		continue;

	case 0xda:
		//case 0x00:
		if (m_f.c) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00da01;
			} else {
				m_ref = 0x00da02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00da03;
			} else {
				m_ref = 0x00da04;
			}
			return;
		}
		//case 0x04:
		PC++;
		PC = TDAT;
			WZ = PC;
		} else {
			// implicit do PC += 2
		//case 0x05:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00da05;
			} else {
				m_ref = 0x00da06;
			}
			return;
		}
		//case 0x06:
		PC++;
		//case 0x07:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00da07;
			} else {
				m_ref = 0x00da08;
			}
			return;
		}
		//case 0x08:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xdb:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00db01;
			} else {
				m_ref = 0x00db02;
			}
			return;
		}
		//case 0x02:
		PC++;
		TDAT2 = TDAT8 | (A << 8);
		//case 0x03:
		TDAT8 = m_io.read_interruptible(TDAT2);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0x00db03;
			} else {
				m_ref = 0x00db04;
			}
			return;
		}
		//case 0x04:
		A = TDAT8;
		WZ = TDAT2 + 1;
		continue;

	case 0xdc:
		//case 0x00:
		if (m_f.c) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00dc01;
			} else {
				m_ref = 0x00dc02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00dc03;
			} else {
				m_ref = 0x00dc04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00dc05;
			return;
		}
		//case 0x05:
		WZ = m_ea;
		SP--;
		//case 0x06:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00dc06;
			} else {
				m_ref = 0x00dc07;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00dc08;
			} else {
				m_ref = 0x00dc09;
			}
			return;
		}
		//case 0x09:
		PC = m_ea;
		} else {
		//case 0x0a:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00dc0a;
			} else {
				m_ref = 0x00dc0b;
			}
			return;
		}
		//case 0x0b:
		PC++;
		//case 0x0c:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00dc0c;
			} else {
				m_ref = 0x00dc0d;
			}
			return;
		}
		//case 0x0d:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xdd:
		//case 0x00:
		//case 0x01:
		TDAT8 = opcode_read();
		m_icount -= m_m1_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_m1_cycles;
				m_ref = 0x00dd01;
			} else {
				m_ref = 0x00dd02;
			}
			return;
		}
		//case 0x02:
		PC++;
		R++;
		Q = QT;
		QT = YXF;
		m_ref = (0xdd << 16) | (TDAT8 << 8);
		goto process;
		continue;

	case 0xde:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00de01;
			} else {
				m_ref = 0x00de02;
			}
			return;
		}
		//case 0x02:
		PC++;
		sbc_a(TDAT8);
		continue;

	case 0xdf:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00df01;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00df02;
			} else {
				m_ref = 0x00df03;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00df04;
			} else {
				m_ref = 0x00df05;
			}
			return;
		}
		//case 0x05:
		PC = 0x18;
		WZ = PC;
		continue;

	case 0xe0:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00e001;
			return;
		}
		//case 0x01:
		if (!m_f.pv()) {
		//case 0x02:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e002;
			} else {
				m_ref = 0x00e003;
			}
			return;
		}
		//case 0x03:
		SP++;
		//case 0x04:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e004;
			} else {
				m_ref = 0x00e005;
			}
			return;
		}
		//case 0x05:
		SP++;
			PC = TDAT;
			WZ = PC;
		}
		continue;

	case 0xe1:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e101;
			} else {
				m_ref = 0x00e102;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e103;
			} else {
				m_ref = 0x00e104;
			}
			return;
		}
		//case 0x04:
		SP++;
		HL = TDAT;
		continue;

	case 0xe2:
		//case 0x00:
		if (!m_f.pv()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e201;
			} else {
				m_ref = 0x00e202;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e203;
			} else {
				m_ref = 0x00e204;
			}
			return;
		}
		//case 0x04:
		PC++;
		PC = TDAT;
			WZ = PC;
		} else {
			// implicit do PC += 2
		//case 0x05:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e205;
			} else {
				m_ref = 0x00e206;
			}
			return;
		}
		//case 0x06:
		PC++;
		//case 0x07:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e207;
			} else {
				m_ref = 0x00e208;
			}
			return;
		}
		//case 0x08:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xe3:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e301;
			} else {
				m_ref = 0x00e302;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e303;
			} else {
				m_ref = 0x00e304;
			}
			return;
		}
		//case 0x04:
		SP++;
		if (nomreq_en)
			m_nomreq_cb(SP-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00e305;
			return;
		}
		//case 0x05:
		SP--;
		//case 0x06:
		stack_write(SP, HL >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e306;
			} else {
				m_ref = 0x00e307;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e308;
			} else {
				m_ref = 0x00e309;
			}
			return;
		}
		//case 0x09:
		HL = TDAT;
		if (nomreq_en)
			m_nomreq_cb(SP, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00e30a;
			return;
		}
		//case 0x0a:
		if (nomreq_en)
			m_nomreq_cb(SP, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00e30b;
			return;
		}
		//case 0x0b:
		WZ = TDAT;
		continue;

	case 0xe4:
		//case 0x00:
		if (!m_f.pv()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e401;
			} else {
				m_ref = 0x00e402;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e403;
			} else {
				m_ref = 0x00e404;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00e405;
			return;
		}
		//case 0x05:
		WZ = m_ea;
		SP--;
		//case 0x06:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e406;
			} else {
				m_ref = 0x00e407;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e408;
			} else {
				m_ref = 0x00e409;
			}
			return;
		}
		//case 0x09:
		PC = m_ea;
		} else {
		//case 0x0a:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e40a;
			} else {
				m_ref = 0x00e40b;
			}
			return;
		}
		//case 0x0b:
		PC++;
		//case 0x0c:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e40c;
			} else {
				m_ref = 0x00e40d;
			}
			return;
		}
		//case 0x0d:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xe5:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00e501;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, HL >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e502;
			} else {
				m_ref = 0x00e503;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e504;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xe6:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e601;
			} else {
				m_ref = 0x00e602;
			}
			return;
		}
		//case 0x02:
		PC++;
		and_a(TDAT8);
		continue;

	case 0xe7:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00e701;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e702;
			} else {
				m_ref = 0x00e703;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e704;
			} else {
				m_ref = 0x00e705;
			}
			return;
		}
		//case 0x05:
		PC = 0x20;
		WZ = PC;
		continue;

	case 0xe8:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00e801;
			return;
		}
		//case 0x01:
		if (m_f.pv()) {
		//case 0x02:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e802;
			} else {
				m_ref = 0x00e803;
			}
			return;
		}
		//case 0x03:
		SP++;
		//case 0x04:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00e804;
			} else {
				m_ref = 0x00e805;
			}
			return;
		}
		//case 0x05:
		SP++;
			PC = TDAT;
			WZ = PC;
		}
		continue;

	case 0xe9:
		PC = HL;
		continue;

	case 0xea:
		//case 0x00:
		if (m_f.pv()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ea01;
			} else {
				m_ref = 0x00ea02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ea03;
			} else {
				m_ref = 0x00ea04;
			}
			return;
		}
		//case 0x04:
		PC++;
		PC = TDAT;
			WZ = PC;
		} else {
			// implicit do PC += 2
		//case 0x05:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ea05;
			} else {
				m_ref = 0x00ea06;
			}
			return;
		}
		//case 0x06:
		PC++;
		//case 0x07:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ea07;
			} else {
				m_ref = 0x00ea08;
			}
			return;
		}
		//case 0x08:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xeb:
		using std::swap;
		swap(DE, HL);
		continue;

	case 0xec:
		//case 0x00:
		if (m_f.pv()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ec01;
			} else {
				m_ref = 0x00ec02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ec03;
			} else {
				m_ref = 0x00ec04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00ec05;
			return;
		}
		//case 0x05:
		WZ = m_ea;
		SP--;
		//case 0x06:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ec06;
			} else {
				m_ref = 0x00ec07;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ec08;
			} else {
				m_ref = 0x00ec09;
			}
			return;
		}
		//case 0x09:
		PC = m_ea;
		} else {
		//case 0x0a:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ec0a;
			} else {
				m_ref = 0x00ec0b;
			}
			return;
		}
		//case 0x0b:
		PC++;
		//case 0x0c:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ec0c;
			} else {
				m_ref = 0x00ec0d;
			}
			return;
		}
		//case 0x0d:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xed:
		//case 0x00:
		//case 0x01:
		TDAT8 = opcode_read();
		m_icount -= m_m1_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_m1_cycles;
				m_ref = 0x00ed01;
			} else {
				m_ref = 0x00ed02;
			}
			return;
		}
		//case 0x02:
		PC++;
		R++;
		Q = QT;
		QT = YXF;
		m_ref = (0xed << 16) | (TDAT8 << 8);
		goto process;
		continue;

	case 0xee:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ee01;
			} else {
				m_ref = 0x00ee02;
			}
			return;
		}
		//case 0x02:
		PC++;
		xor_a(TDAT8);
		continue;

	case 0xef:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00ef01;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ef02;
			} else {
				m_ref = 0x00ef03;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ef04;
			} else {
				m_ref = 0x00ef05;
			}
			return;
		}
		//case 0x05:
		PC = 0x28;
		WZ = PC;
		continue;

	case 0xf0:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00f001;
			return;
		}
		//case 0x01:
		if (!m_f.s()) {
		//case 0x02:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f002;
			} else {
				m_ref = 0x00f003;
			}
			return;
		}
		//case 0x03:
		SP++;
		//case 0x04:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f004;
			} else {
				m_ref = 0x00f005;
			}
			return;
		}
		//case 0x05:
		SP++;
			PC = TDAT;
			WZ = PC;
		}
		continue;

	case 0xf1:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f101;
			} else {
				m_ref = 0x00f102;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f103;
			} else {
				m_ref = 0x00f104;
			}
			return;
		}
		//case 0x04:
		SP++;
		A = TDAT_H;
		set_f(TDAT_L);
		continue;

	case 0xf2:
		//case 0x00:
		if (!m_f.s()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f201;
			} else {
				m_ref = 0x00f202;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f203;
			} else {
				m_ref = 0x00f204;
			}
			return;
		}
		//case 0x04:
		PC++;
		PC = TDAT;
			WZ = PC;
		} else {
			// implicit do PC += 2
		//case 0x05:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f205;
			} else {
				m_ref = 0x00f206;
			}
			return;
		}
		//case 0x06:
		PC++;
		//case 0x07:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f207;
			} else {
				m_ref = 0x00f208;
			}
			return;
		}
		//case 0x08:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xf3:
		//case 0x00:
		m_iff1 = m_iff2 = 0;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xf4:
		//case 0x00:
		if (!m_f.s()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f401;
			} else {
				m_ref = 0x00f402;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f403;
			} else {
				m_ref = 0x00f404;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00f405;
			return;
		}
		//case 0x05:
		WZ = m_ea;
		SP--;
		//case 0x06:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f406;
			} else {
				m_ref = 0x00f407;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f408;
			} else {
				m_ref = 0x00f409;
			}
			return;
		}
		//case 0x09:
		PC = m_ea;
		} else {
		//case 0x0a:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f40a;
			} else {
				m_ref = 0x00f40b;
			}
			return;
		}
		//case 0x0b:
		PC++;
		//case 0x0c:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f40c;
			} else {
				m_ref = 0x00f40d;
			}
			return;
		}
		//case 0x0d:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xf5:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00f501;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, A);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f502;
			} else {
				m_ref = 0x00f503;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, get_f());
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f504;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xf6:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f601;
			} else {
				m_ref = 0x00f602;
			}
			return;
		}
		//case 0x02:
		PC++;
		or_a(TDAT8);
		continue;

	case 0xf7:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00f701;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f702;
			} else {
				m_ref = 0x00f703;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f704;
			} else {
				m_ref = 0x00f705;
			}
			return;
		}
		//case 0x05:
		PC = 0x30;
		WZ = PC;
		continue;

	case 0xf8:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00f801;
			return;
		}
		//case 0x01:
		if (m_f.s()) {
		//case 0x02:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f802;
			} else {
				m_ref = 0x00f803;
			}
			return;
		}
		//case 0x03:
		SP++;
		//case 0x04:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00f804;
			} else {
				m_ref = 0x00f805;
			}
			return;
		}
		//case 0x05:
		SP++;
			PC = TDAT;
			WZ = PC;
		}
		continue;

	case 0xf9:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00f901;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00f902;
			return;
		}
		//case 0x02:
		SP = HL;
		continue;

	case 0xfa:
		//case 0x00:
		if (m_f.s()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fa01;
			} else {
				m_ref = 0x00fa02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fa03;
			} else {
				m_ref = 0x00fa04;
			}
			return;
		}
		//case 0x04:
		PC++;
		PC = TDAT;
			WZ = PC;
		} else {
			// implicit do PC += 2
		//case 0x05:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fa05;
			} else {
				m_ref = 0x00fa06;
			}
			return;
		}
		//case 0x06:
		PC++;
		//case 0x07:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fa07;
			} else {
				m_ref = 0x00fa08;
			}
			return;
		}
		//case 0x08:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xfb:
		ei();
		continue;

	case 0xfc:
		//case 0x00:
		if (m_f.s()) {
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fc01;
			} else {
				m_ref = 0x00fc02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fc03;
			} else {
				m_ref = 0x00fc04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00fc05;
			return;
		}
		//case 0x05:
		WZ = m_ea;
		SP--;
		//case 0x06:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fc06;
			} else {
				m_ref = 0x00fc07;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fc08;
			} else {
				m_ref = 0x00fc09;
			}
			return;
		}
		//case 0x09:
		PC = m_ea;
		} else {
		//case 0x0a:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fc0a;
			} else {
				m_ref = 0x00fc0b;
			}
			return;
		}
		//case 0x0b:
		PC++;
		//case 0x0c:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fc0c;
			} else {
				m_ref = 0x00fc0d;
			}
			return;
		}
		//case 0x0d:
		PC++;
		WZ = TDAT;
		}
		continue;

	case 0xfd:
		//case 0x00:
		//case 0x01:
		TDAT8 = opcode_read();
		m_icount -= m_m1_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_m1_cycles;
				m_ref = 0x00fd01;
			} else {
				m_ref = 0x00fd02;
			}
			return;
		}
		//case 0x02:
		PC++;
		R++;
		Q = QT;
		QT = YXF;
		m_ref = (0xfd << 16) | (TDAT8 << 8);
		goto process;
		continue;

	case 0xfe:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00fe01;
			} else {
				m_ref = 0x00fe02;
			}
			return;
		}
		//case 0x02:
		PC++;
		cp(TDAT8);
		continue;

	case 0xff:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0x00ff01;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, PC >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ff02;
			} else {
				m_ref = 0x00ff03;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, PC);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0x00ff04;
			} else {
				m_ref = 0x00ff05;
			}
			return;
		}
		//case 0x05:
		PC = 0x38;
		WZ = PC;
		continue;

	}
} break; // prefix: 00

case 0xcb:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x00:
		B = rlc(B);
		continue;

	case 0x01:
		C = rlc(C);
		continue;

	case 0x02:
		D = rlc(D);
		continue;

	case 0x03:
		E = rlc(E);
		continue;

	case 0x04:
		H = rlc(H);
		continue;

	case 0x05:
		L = rlc(L);
		continue;

	case 0x06:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb0601;
			} else {
				m_ref = 0xcb0602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb0603;
			return;
		}
		//case 0x03:
		TDAT8 = rlc(TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb0604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x07:
		A = rlc(A);
		continue;

	case 0x08:
		B = rrc(B);
		continue;

	case 0x09:
		C = rrc(C);
		continue;

	case 0x0a:
		D = rrc(D);
		continue;

	case 0x0b:
		E = rrc(E);
		continue;

	case 0x0c:
		H = rrc(H);
		continue;

	case 0x0d:
		L = rrc(L);
		continue;

	case 0x0e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb0e01;
			} else {
				m_ref = 0xcb0e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb0e03;
			return;
		}
		//case 0x03:
		TDAT8 = rrc(TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb0e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x0f:
		A = rrc(A);
		continue;

	case 0x10:
		B = rl(B);
		continue;

	case 0x11:
		C = rl(C);
		continue;

	case 0x12:
		D = rl(D);
		continue;

	case 0x13:
		E = rl(E);
		continue;

	case 0x14:
		H = rl(H);
		continue;

	case 0x15:
		L = rl(L);
		continue;

	case 0x16:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb1601;
			} else {
				m_ref = 0xcb1602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb1603;
			return;
		}
		//case 0x03:
		TDAT8 = rl(TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb1604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x17:
		A = rl(A);
		continue;

	case 0x18:
		B = rr(B);
		continue;

	case 0x19:
		C = rr(C);
		continue;

	case 0x1a:
		D = rr(D);
		continue;

	case 0x1b:
		E = rr(E);
		continue;

	case 0x1c:
		H = rr(H);
		continue;

	case 0x1d:
		L = rr(L);
		continue;

	case 0x1e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb1e01;
			} else {
				m_ref = 0xcb1e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb1e03;
			return;
		}
		//case 0x03:
		TDAT8 = rr(TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb1e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x1f:
		A = rr(A);
		continue;

	case 0x20:
		B = sla(B);
		continue;

	case 0x21:
		C = sla(C);
		continue;

	case 0x22:
		D = sla(D);
		continue;

	case 0x23:
		E = sla(E);
		continue;

	case 0x24:
		H = sla(H);
		continue;

	case 0x25:
		L = sla(L);
		continue;

	case 0x26:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb2601;
			} else {
				m_ref = 0xcb2602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb2603;
			return;
		}
		//case 0x03:
		TDAT8 = sla(TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb2604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x27:
		A = sla(A);
		continue;

	case 0x28:
		B = sra(B);
		continue;

	case 0x29:
		C = sra(C);
		continue;

	case 0x2a:
		D = sra(D);
		continue;

	case 0x2b:
		E = sra(E);
		continue;

	case 0x2c:
		H = sra(H);
		continue;

	case 0x2d:
		L = sra(L);
		continue;

	case 0x2e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb2e01;
			} else {
				m_ref = 0xcb2e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb2e03;
			return;
		}
		//case 0x03:
		TDAT8 = sra(TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb2e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x2f:
		A = sra(A);
		continue;

	case 0x30:
		B = r800_sll(B);
		continue;

	case 0x31:
		C = r800_sll(C);
		continue;

	case 0x32:
		D = r800_sll(D);
		continue;

	case 0x33:
		E = r800_sll(E);
		continue;

	case 0x34:
		H = r800_sll(H);
		continue;

	case 0x35:
		L = r800_sll(L);
		continue;

	case 0x36:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb3601;
			} else {
				m_ref = 0xcb3602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb3603;
			return;
		}
		//case 0x03:
		TDAT8 = r800_sll(TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb3604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x37:
		A = r800_sll(A);
		continue;

	case 0x38:
		B = srl(B);
		continue;

	case 0x39:
		C = srl(C);
		continue;

	case 0x3a:
		D = srl(D);
		continue;

	case 0x3b:
		E = srl(E);
		continue;

	case 0x3c:
		H = srl(H);
		continue;

	case 0x3d:
		L = srl(L);
		continue;

	case 0x3e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb3e01;
			} else {
				m_ref = 0xcb3e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb3e03;
			return;
		}
		//case 0x03:
		TDAT8 = srl(TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb3e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x3f:
		A = srl(A);
		continue;

	case 0x40:
		bit(0, B);
		continue;

	case 0x41:
		bit(0, C);
		continue;

	case 0x42:
		bit(0, D);
		continue;

	case 0x43:
		bit(0, E);
		continue;

	case 0x44:
		bit(0, H);
		continue;

	case 0x45:
		bit(0, L);
		continue;

	case 0x46:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb4601;
			} else {
				m_ref = 0xcb4602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb4603;
			return;
		}
		//case 0x03:
		bit_hl(0, TDAT8);
		continue;

	case 0x47:
		bit(0, A);
		continue;

	case 0x48:
		bit(1, B);
		continue;

	case 0x49:
		bit(1, C);
		continue;

	case 0x4a:
		bit(1, D);
		continue;

	case 0x4b:
		bit(1, E);
		continue;

	case 0x4c:
		bit(1, H);
		continue;

	case 0x4d:
		bit(1, L);
		continue;

	case 0x4e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb4e01;
			} else {
				m_ref = 0xcb4e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb4e03;
			return;
		}
		//case 0x03:
		bit_hl(1, TDAT8);
		continue;

	case 0x4f:
		bit(1, A);
		continue;

	case 0x50:
		bit(2, B);
		continue;

	case 0x51:
		bit(2, C);
		continue;

	case 0x52:
		bit(2, D);
		continue;

	case 0x53:
		bit(2, E);
		continue;

	case 0x54:
		bit(2, H);
		continue;

	case 0x55:
		bit(2, L);
		continue;

	case 0x56:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb5601;
			} else {
				m_ref = 0xcb5602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb5603;
			return;
		}
		//case 0x03:
		bit_hl(2, TDAT8);
		continue;

	case 0x57:
		bit(2, A);
		continue;

	case 0x58:
		bit(3, B);
		continue;

	case 0x59:
		bit(3, C);
		continue;

	case 0x5a:
		bit(3, D);
		continue;

	case 0x5b:
		bit(3, E);
		continue;

	case 0x5c:
		bit(3, H);
		continue;

	case 0x5d:
		bit(3, L);
		continue;

	case 0x5e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb5e01;
			} else {
				m_ref = 0xcb5e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb5e03;
			return;
		}
		//case 0x03:
		bit_hl(3, TDAT8);
		continue;

	case 0x5f:
		bit(3, A);
		continue;

	case 0x60:
		bit(4, B);
		continue;

	case 0x61:
		bit(4, C);
		continue;

	case 0x62:
		bit(4, D);
		continue;

	case 0x63:
		bit(4, E);
		continue;

	case 0x64:
		bit(4, H);
		continue;

	case 0x65:
		bit(4, L);
		continue;

	case 0x66:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb6601;
			} else {
				m_ref = 0xcb6602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb6603;
			return;
		}
		//case 0x03:
		bit_hl(4, TDAT8);
		continue;

	case 0x67:
		bit(4, A);
		continue;

	case 0x68:
		bit(5, B);
		continue;

	case 0x69:
		bit(5, C);
		continue;

	case 0x6a:
		bit(5, D);
		continue;

	case 0x6b:
		bit(5, E);
		continue;

	case 0x6c:
		bit(5, H);
		continue;

	case 0x6d:
		bit(5, L);
		continue;

	case 0x6e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb6e01;
			} else {
				m_ref = 0xcb6e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb6e03;
			return;
		}
		//case 0x03:
		bit_hl(5, TDAT8);
		continue;

	case 0x6f:
		bit(5, A);
		continue;

	case 0x70:
		bit(6, B);
		continue;

	case 0x71:
		bit(6, C);
		continue;

	case 0x72:
		bit(6, D);
		continue;

	case 0x73:
		bit(6, E);
		continue;

	case 0x74:
		bit(6, H);
		continue;

	case 0x75:
		bit(6, L);
		continue;

	case 0x76:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb7601;
			} else {
				m_ref = 0xcb7602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb7603;
			return;
		}
		//case 0x03:
		bit_hl(6, TDAT8);
		continue;

	case 0x77:
		bit(6, A);
		continue;

	case 0x78:
		bit(7, B);
		continue;

	case 0x79:
		bit(7, C);
		continue;

	case 0x7a:
		bit(7, D);
		continue;

	case 0x7b:
		bit(7, E);
		continue;

	case 0x7c:
		bit(7, H);
		continue;

	case 0x7d:
		bit(7, L);
		continue;

	case 0x7e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb7e01;
			} else {
				m_ref = 0xcb7e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb7e03;
			return;
		}
		//case 0x03:
		bit_hl(7, TDAT8);
		continue;

	case 0x7f:
		bit(7, A);
		continue;

	case 0x80:
		B = res(0, B);
		continue;

	case 0x81:
		C = res(0, C);
		continue;

	case 0x82:
		D = res(0, D);
		continue;

	case 0x83:
		E = res(0, E);
		continue;

	case 0x84:
		H = res(0, H);
		continue;

	case 0x85:
		L = res(0, L);
		continue;

	case 0x86:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb8601;
			} else {
				m_ref = 0xcb8602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb8603;
			return;
		}
		//case 0x03:
		TDAT8 = res(0, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb8604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x87:
		A = res(0, A);
		continue;

	case 0x88:
		B = res(1, B);
		continue;

	case 0x89:
		C = res(1, C);
		continue;

	case 0x8a:
		D = res(1, D);
		continue;

	case 0x8b:
		E = res(1, E);
		continue;

	case 0x8c:
		H = res(1, H);
		continue;

	case 0x8d:
		L = res(1, L);
		continue;

	case 0x8e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb8e01;
			} else {
				m_ref = 0xcb8e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb8e03;
			return;
		}
		//case 0x03:
		TDAT8 = res(1, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb8e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x8f:
		A = res(1, A);
		continue;

	case 0x90:
		B = res(2, B);
		continue;

	case 0x91:
		C = res(2, C);
		continue;

	case 0x92:
		D = res(2, D);
		continue;

	case 0x93:
		E = res(2, E);
		continue;

	case 0x94:
		H = res(2, H);
		continue;

	case 0x95:
		L = res(2, L);
		continue;

	case 0x96:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb9601;
			} else {
				m_ref = 0xcb9602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb9603;
			return;
		}
		//case 0x03:
		TDAT8 = res(2, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb9604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x97:
		A = res(2, A);
		continue;

	case 0x98:
		B = res(3, B);
		continue;

	case 0x99:
		C = res(3, C);
		continue;

	case 0x9a:
		D = res(3, D);
		continue;

	case 0x9b:
		E = res(3, E);
		continue;

	case 0x9c:
		H = res(3, H);
		continue;

	case 0x9d:
		L = res(3, L);
		continue;

	case 0x9e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb9e01;
			} else {
				m_ref = 0xcb9e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcb9e03;
			return;
		}
		//case 0x03:
		TDAT8 = res(3, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcb9e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x9f:
		A = res(3, A);
		continue;

	case 0xa0:
		B = res(4, B);
		continue;

	case 0xa1:
		C = res(4, C);
		continue;

	case 0xa2:
		D = res(4, D);
		continue;

	case 0xa3:
		E = res(4, E);
		continue;

	case 0xa4:
		H = res(4, H);
		continue;

	case 0xa5:
		L = res(4, L);
		continue;

	case 0xa6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcba601;
			} else {
				m_ref = 0xcba602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcba603;
			return;
		}
		//case 0x03:
		TDAT8 = res(4, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcba604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xa7:
		A = res(4, A);
		continue;

	case 0xa8:
		B = res(5, B);
		continue;

	case 0xa9:
		C = res(5, C);
		continue;

	case 0xaa:
		D = res(5, D);
		continue;

	case 0xab:
		E = res(5, E);
		continue;

	case 0xac:
		H = res(5, H);
		continue;

	case 0xad:
		L = res(5, L);
		continue;

	case 0xae:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbae01;
			} else {
				m_ref = 0xcbae02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbae03;
			return;
		}
		//case 0x03:
		TDAT8 = res(5, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbae04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xaf:
		A = res(5, A);
		continue;

	case 0xb0:
		B = res(6, B);
		continue;

	case 0xb1:
		C = res(6, C);
		continue;

	case 0xb2:
		D = res(6, D);
		continue;

	case 0xb3:
		E = res(6, E);
		continue;

	case 0xb4:
		H = res(6, H);
		continue;

	case 0xb5:
		L = res(6, L);
		continue;

	case 0xb6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbb601;
			} else {
				m_ref = 0xcbb602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbb603;
			return;
		}
		//case 0x03:
		TDAT8 = res(6, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbb604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xb7:
		A = res(6, A);
		continue;

	case 0xb8:
		B = res(7, B);
		continue;

	case 0xb9:
		C = res(7, C);
		continue;

	case 0xba:
		D = res(7, D);
		continue;

	case 0xbb:
		E = res(7, E);
		continue;

	case 0xbc:
		H = res(7, H);
		continue;

	case 0xbd:
		L = res(7, L);
		continue;

	case 0xbe:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbbe01;
			} else {
				m_ref = 0xcbbe02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbbe03;
			return;
		}
		//case 0x03:
		TDAT8 = res(7, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbbe04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xbf:
		A = res(7, A);
		continue;

	case 0xc0:
		B = set(0, B);
		continue;

	case 0xc1:
		C = set(0, C);
		continue;

	case 0xc2:
		D = set(0, D);
		continue;

	case 0xc3:
		E = set(0, E);
		continue;

	case 0xc4:
		H = set(0, H);
		continue;

	case 0xc5:
		L = set(0, L);
		continue;

	case 0xc6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbc601;
			} else {
				m_ref = 0xcbc602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbc603;
			return;
		}
		//case 0x03:
		TDAT8 = set(0, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbc604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xc7:
		A = set(0, A);
		continue;

	case 0xc8:
		B = set(1, B);
		continue;

	case 0xc9:
		C = set(1, C);
		continue;

	case 0xca:
		D = set(1, D);
		continue;

	case 0xcb:
		E = set(1, E);
		continue;

	case 0xcc:
		H = set(1, H);
		continue;

	case 0xcd:
		L = set(1, L);
		continue;

	case 0xce:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbce01;
			} else {
				m_ref = 0xcbce02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbce03;
			return;
		}
		//case 0x03:
		TDAT8 = set(1, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbce04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xcf:
		A = set(1, A);
		continue;

	case 0xd0:
		B = set(2, B);
		continue;

	case 0xd1:
		C = set(2, C);
		continue;

	case 0xd2:
		D = set(2, D);
		continue;

	case 0xd3:
		E = set(2, E);
		continue;

	case 0xd4:
		H = set(2, H);
		continue;

	case 0xd5:
		L = set(2, L);
		continue;

	case 0xd6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbd601;
			} else {
				m_ref = 0xcbd602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbd603;
			return;
		}
		//case 0x03:
		TDAT8 = set(2, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbd604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xd7:
		A = set(2, A);
		continue;

	case 0xd8:
		B = set(3, B);
		continue;

	case 0xd9:
		C = set(3, C);
		continue;

	case 0xda:
		D = set(3, D);
		continue;

	case 0xdb:
		E = set(3, E);
		continue;

	case 0xdc:
		H = set(3, H);
		continue;

	case 0xdd:
		L = set(3, L);
		continue;

	case 0xde:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbde01;
			} else {
				m_ref = 0xcbde02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbde03;
			return;
		}
		//case 0x03:
		TDAT8 = set(3, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbde04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xdf:
		A = set(3, A);
		continue;

	case 0xe0:
		B = set(4, B);
		continue;

	case 0xe1:
		C = set(4, C);
		continue;

	case 0xe2:
		D = set(4, D);
		continue;

	case 0xe3:
		E = set(4, E);
		continue;

	case 0xe4:
		H = set(4, H);
		continue;

	case 0xe5:
		L = set(4, L);
		continue;

	case 0xe6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbe601;
			} else {
				m_ref = 0xcbe602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbe603;
			return;
		}
		//case 0x03:
		TDAT8 = set(4, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbe604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xe7:
		A = set(4, A);
		continue;

	case 0xe8:
		B = set(5, B);
		continue;

	case 0xe9:
		C = set(5, C);
		continue;

	case 0xea:
		D = set(5, D);
		continue;

	case 0xeb:
		E = set(5, E);
		continue;

	case 0xec:
		H = set(5, H);
		continue;

	case 0xed:
		L = set(5, L);
		continue;

	case 0xee:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbee01;
			} else {
				m_ref = 0xcbee02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbee03;
			return;
		}
		//case 0x03:
		TDAT8 = set(5, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbee04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xef:
		A = set(5, A);
		continue;

	case 0xf0:
		B = set(6, B);
		continue;

	case 0xf1:
		C = set(6, C);
		continue;

	case 0xf2:
		D = set(6, D);
		continue;

	case 0xf3:
		E = set(6, E);
		continue;

	case 0xf4:
		H = set(6, H);
		continue;

	case 0xf5:
		L = set(6, L);
		continue;

	case 0xf6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbf601;
			} else {
				m_ref = 0xcbf602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbf603;
			return;
		}
		//case 0x03:
		TDAT8 = set(6, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbf604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xf7:
		A = set(6, A);
		continue;

	case 0xf8:
		B = set(7, B);
		continue;

	case 0xf9:
		C = set(7, C);
		continue;

	case 0xfa:
		D = set(7, D);
		continue;

	case 0xfb:
		E = set(7, E);
		continue;

	case 0xfc:
		H = set(7, H);
		continue;

	case 0xfd:
		L = set(7, L);
		continue;

	case 0xfe:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbfe01;
			} else {
				m_ref = 0xcbfe02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xcbfe03;
			return;
		}
		//case 0x03:
		TDAT8 = set(7, TDAT8);
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xcbfe04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xff:
		A = set(7, A);
		continue;

	}
} break; // prefix: cb

case 0xdd:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x00:
		illegal_1();
		continue;

	case 0x01:
		illegal_1();
		continue;

	case 0x02:
		illegal_1();
		continue;

	case 0x03:
		illegal_1();
		continue;

	case 0x04:
		illegal_1();
		continue;

	case 0x05:
		illegal_1();
		continue;

	case 0x06:
		illegal_1();
		continue;

	case 0x07:
		illegal_1();
		continue;

	case 0x08:
		illegal_1();
		continue;

	case 0x09:
		{
		const u32 res = IX +  BC;
		WZ = IX + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (IX ^ res ^  BC) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		IX = (u16)res;
		}
		continue;

	case 0x0a:
		illegal_1();
		continue;

	case 0x0b:
		illegal_1();
		continue;

	case 0x0c:
		illegal_1();
		continue;

	case 0x0d:
		illegal_1();
		continue;

	case 0x0e:
		illegal_1();
		continue;

	case 0x0f:
		illegal_1();
		continue;

	case 0x10:
		illegal_1();
		continue;

	case 0x11:
		illegal_1();
		continue;

	case 0x12:
		illegal_1();
		continue;

	case 0x13:
		illegal_1();
		continue;

	case 0x14:
		illegal_1();
		continue;

	case 0x15:
		illegal_1();
		continue;

	case 0x16:
		illegal_1();
		continue;

	case 0x17:
		illegal_1();
		continue;

	case 0x18:
		illegal_1();
		continue;

	case 0x19:
		{
		const u32 res = IX +  DE;
		WZ = IX + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (IX ^ res ^  DE) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		IX = (u16)res;
		}
		continue;

	case 0x1a:
		illegal_1();
		continue;

	case 0x1b:
		illegal_1();
		continue;

	case 0x1c:
		illegal_1();
		continue;

	case 0x1d:
		illegal_1();
		continue;

	case 0x1e:
		illegal_1();
		continue;

	case 0x1f:
		illegal_1();
		continue;

	case 0x20:
		illegal_1();
		continue;

	case 0x21:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2101;
			} else {
				m_ref = 0xdd2102;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2103;
			} else {
				m_ref = 0xdd2104;
			}
			return;
		}
		//case 0x04:
		PC++;
		IX = TDAT;
		continue;

	case 0x22:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2201;
			} else {
				m_ref = 0xdd2202;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2203;
			} else {
				m_ref = 0xdd2204;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		TDAT = IX;
		//case 0x05:
		data_write(m_ea, TDAT_L);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2205;
			} else {
				m_ref = 0xdd2206;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		data_write(m_ea+1, TDAT_H);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2207;
			} else {
				m_ref = 0xdd2208;
			}
			return;
		}
		//case 0x08:
		WZ = m_ea + 1;
		continue;

	case 0x23:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd2301;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd2302;
			return;
		}
		//case 0x02:
		IX++;
		continue;

	case 0x24:
		inc(HX);
		continue;

	case 0x25:
		dec(HX);
		continue;

	case 0x26:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2601;
			} else {
				m_ref = 0xdd2602;
			}
			return;
		}
		//case 0x02:
		PC++;
		HX = TDAT8;
		continue;

	case 0x27:
		illegal_1();
		continue;

	case 0x28:
		illegal_1();
		continue;

	case 0x29:
		{
		const u32 res = IX +  IX;
		WZ = IX + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (IX ^ res ^  IX) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		IX = (u16)res;
		}
		continue;

	case 0x2a:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2a01;
			} else {
				m_ref = 0xdd2a02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2a03;
			} else {
				m_ref = 0xdd2a04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		//case 0x05:
		TDAT_L = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2a05;
			} else {
				m_ref = 0xdd2a06;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		TDAT_H = data_read(m_ea+1);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2a07;
			} else {
				m_ref = 0xdd2a08;
			}
			return;
		}
		//case 0x08:
		IX = TDAT;
		WZ = m_ea + 1;
		continue;

	case 0x2b:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd2b01;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd2b02;
			return;
		}
		//case 0x02:
		IX--;
		continue;

	case 0x2c:
		inc(LX);
		continue;

	case 0x2d:
		dec(LX);
		continue;

	case 0x2e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd2e01;
			} else {
				m_ref = 0xdd2e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		LX = TDAT8;
		continue;

	case 0x2f:
		illegal_1();
		continue;

	case 0x30:
		illegal_1();
		continue;

	case 0x31:
		illegal_1();
		continue;

	case 0x32:
		illegal_1();
		continue;

	case 0x33:
		illegal_1();
		continue;

	case 0x34:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd3401;
			} else {
				m_ref = 0xdd3402;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3403;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3404;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3405;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3406;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3407;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd3408;
			} else {
				m_ref = 0xdd3409;
			}
			return;
		}
		//case 0x09:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd340a;
			return;
		}
		//case 0x0a:
		inc(TDAT8);
		//case 0x0b:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd340b;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x35:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd3501;
			} else {
				m_ref = 0xdd3502;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3503;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3504;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3505;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3506;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3507;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd3508;
			} else {
				m_ref = 0xdd3509;
			}
			return;
		}
		//case 0x09:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd350a;
			return;
		}
		//case 0x0a:
		dec(TDAT8);
		//case 0x0b:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd350b;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x36:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd3601;
			} else {
				m_ref = 0xdd3602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		//case 0x03:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd3603;
			} else {
				m_ref = 0xdd3604;
			}
			return;
		}
		//case 0x04:
		PC++;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd3606;
			return;
		}
		//case 0x06:
		//case 0x07:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd3607;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x37:
		illegal_1();
		continue;

	case 0x38:
		illegal_1();
		continue;

	case 0x39:
		{
		const u32 res = IX +  SP;
		WZ = IX + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (IX ^ res ^  SP) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		IX = (u16)res;
		}
		continue;

	case 0x3a:
		illegal_1();
		continue;

	case 0x3b:
		illegal_1();
		continue;

	case 0x3c:
		illegal_1();
		continue;

	case 0x3d:
		illegal_1();
		continue;

	case 0x3e:
		illegal_1();
		continue;

	case 0x3f:
		illegal_1();
		continue;

	case 0x40:
		illegal_1();
		continue;

	case 0x41:
		illegal_1();
		continue;

	case 0x42:
		illegal_1();
		continue;

	case 0x43:
		illegal_1();
		continue;

	case 0x44:
		B = HX;
		continue;

	case 0x45:
		B = LX;
		continue;

	case 0x46:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd4601;
			} else {
				m_ref = 0xdd4602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd4603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd4604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd4605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd4606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd4607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd4608;
			} else {
				m_ref = 0xdd4609;
			}
			return;
		}
		//case 0x09:
		B = TDAT8;
		continue;

	case 0x47:
		illegal_1();
		continue;

	case 0x48:
		illegal_1();
		continue;

	case 0x49:
		illegal_1();
		continue;

	case 0x4a:
		illegal_1();
		continue;

	case 0x4b:
		illegal_1();
		continue;

	case 0x4c:
		C = HX;
		continue;

	case 0x4d:
		C = LX;
		continue;

	case 0x4e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd4e01;
			} else {
				m_ref = 0xdd4e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd4e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd4e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd4e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd4e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd4e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd4e08;
			} else {
				m_ref = 0xdd4e09;
			}
			return;
		}
		//case 0x09:
		C = TDAT8;
		continue;

	case 0x4f:
		illegal_1();
		continue;

	case 0x50:
		illegal_1();
		continue;

	case 0x51:
		illegal_1();
		continue;

	case 0x52:
		illegal_1();
		continue;

	case 0x53:
		illegal_1();
		continue;

	case 0x54:
		D = HX;
		continue;

	case 0x55:
		D = LX;
		continue;

	case 0x56:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd5601;
			} else {
				m_ref = 0xdd5602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd5603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd5604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd5605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd5606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd5607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd5608;
			} else {
				m_ref = 0xdd5609;
			}
			return;
		}
		//case 0x09:
		D = TDAT8;
		continue;

	case 0x57:
		illegal_1();
		continue;

	case 0x58:
		illegal_1();
		continue;

	case 0x59:
		illegal_1();
		continue;

	case 0x5a:
		illegal_1();
		continue;

	case 0x5b:
		illegal_1();
		continue;

	case 0x5c:
		E = HX;
		continue;

	case 0x5d:
		E = LX;
		continue;

	case 0x5e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd5e01;
			} else {
				m_ref = 0xdd5e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd5e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd5e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd5e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd5e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd5e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd5e08;
			} else {
				m_ref = 0xdd5e09;
			}
			return;
		}
		//case 0x09:
		E = TDAT8;
		continue;

	case 0x5f:
		illegal_1();
		continue;

	case 0x60:
		HX = B;
		continue;

	case 0x61:
		HX = C;
		continue;

	case 0x62:
		HX = D;
		continue;

	case 0x63:
		HX = E;
		continue;

	case 0x64:
		continue;

	case 0x65:
		HX = LX;
		continue;

	case 0x66:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd6601;
			} else {
				m_ref = 0xdd6602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd6603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd6604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd6605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd6606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd6607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd6608;
			} else {
				m_ref = 0xdd6609;
			}
			return;
		}
		//case 0x09:
		H = TDAT8;
		continue;

	case 0x67:
		HX = A;
		continue;

	case 0x68:
		LX = B;
		continue;

	case 0x69:
		LX = C;
		continue;

	case 0x6a:
		LX = D;
		continue;

	case 0x6b:
		LX = E;
		continue;

	case 0x6c:
		LX = HX;
		continue;

	case 0x6d:
		continue;

	case 0x6e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd6e01;
			} else {
				m_ref = 0xdd6e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd6e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd6e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd6e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd6e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd6e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd6e08;
			} else {
				m_ref = 0xdd6e09;
			}
			return;
		}
		//case 0x09:
		L = TDAT8;
		continue;

	case 0x6f:
		LX = A;
		continue;

	case 0x70:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7001;
			} else {
				m_ref = 0xdd7002;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7003;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7004;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7005;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7006;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7007;
			return;
		}
		//case 0x07:
		TDAT8 = B;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7008;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x71:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7101;
			} else {
				m_ref = 0xdd7102;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7103;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7104;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7105;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7106;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7107;
			return;
		}
		//case 0x07:
		TDAT8 = C;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7108;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x72:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7201;
			} else {
				m_ref = 0xdd7202;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7203;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7204;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7205;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7206;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7207;
			return;
		}
		//case 0x07:
		TDAT8 = D;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7208;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x73:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7301;
			} else {
				m_ref = 0xdd7302;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7303;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7304;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7305;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7306;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7307;
			return;
		}
		//case 0x07:
		TDAT8 = E;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7308;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x74:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7401;
			} else {
				m_ref = 0xdd7402;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7403;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7404;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7405;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7406;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7407;
			return;
		}
		//case 0x07:
		TDAT8 = H;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7408;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x75:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7501;
			} else {
				m_ref = 0xdd7502;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7503;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7504;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7505;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7506;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7507;
			return;
		}
		//case 0x07:
		TDAT8 = L;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7508;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x76:
		illegal_1();
		continue;

	case 0x77:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7701;
			} else {
				m_ref = 0xdd7702;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7703;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7704;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7705;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7706;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7707;
			return;
		}
		//case 0x07:
		TDAT8 = A;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7708;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x78:
		illegal_1();
		continue;

	case 0x79:
		illegal_1();
		continue;

	case 0x7a:
		illegal_1();
		continue;

	case 0x7b:
		illegal_1();
		continue;

	case 0x7c:
		A = HX;
		continue;

	case 0x7d:
		A = LX;
		continue;

	case 0x7e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7e01;
			} else {
				m_ref = 0xdd7e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd7e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd7e08;
			} else {
				m_ref = 0xdd7e09;
			}
			return;
		}
		//case 0x09:
		A = TDAT8;
		continue;

	case 0x7f:
		illegal_1();
		continue;

	case 0x80:
		illegal_1();
		continue;

	case 0x81:
		illegal_1();
		continue;

	case 0x82:
		illegal_1();
		continue;

	case 0x83:
		illegal_1();
		continue;

	case 0x84:
		add_a(HX);
		continue;

	case 0x85:
		add_a(LX);
		continue;

	case 0x86:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd8601;
			} else {
				m_ref = 0xdd8602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd8603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd8604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd8605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd8606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd8607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd8608;
			} else {
				m_ref = 0xdd8609;
			}
			return;
		}
		//case 0x09:
		add_a(TDAT8);
		continue;

	case 0x87:
		illegal_1();
		continue;

	case 0x88:
		illegal_1();
		continue;

	case 0x89:
		illegal_1();
		continue;

	case 0x8a:
		illegal_1();
		continue;

	case 0x8b:
		illegal_1();
		continue;

	case 0x8c:
		adc_a(HX);
		continue;

	case 0x8d:
		adc_a(LX);
		continue;

	case 0x8e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd8e01;
			} else {
				m_ref = 0xdd8e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd8e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd8e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd8e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd8e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd8e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd8e08;
			} else {
				m_ref = 0xdd8e09;
			}
			return;
		}
		//case 0x09:
		adc_a(TDAT8);
		continue;

	case 0x8f:
		illegal_1();
		continue;

	case 0x90:
		illegal_1();
		continue;

	case 0x91:
		illegal_1();
		continue;

	case 0x92:
		illegal_1();
		continue;

	case 0x93:
		illegal_1();
		continue;

	case 0x94:
		sub_a(HX);
		continue;

	case 0x95:
		sub_a(LX);
		continue;

	case 0x96:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd9601;
			} else {
				m_ref = 0xdd9602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd9603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd9604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd9605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd9606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd9607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd9608;
			} else {
				m_ref = 0xdd9609;
			}
			return;
		}
		//case 0x09:
		sub_a(TDAT8);
		continue;

	case 0x97:
		illegal_1();
		continue;

	case 0x98:
		illegal_1();
		continue;

	case 0x99:
		illegal_1();
		continue;

	case 0x9a:
		illegal_1();
		continue;

	case 0x9b:
		illegal_1();
		continue;

	case 0x9c:
		sbc_a(HX);
		continue;

	case 0x9d:
		sbc_a(LX);
		continue;

	case 0x9e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd9e01;
			} else {
				m_ref = 0xdd9e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd9e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd9e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd9e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd9e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdd9e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdd9e08;
			} else {
				m_ref = 0xdd9e09;
			}
			return;
		}
		//case 0x09:
		sbc_a(TDAT8);
		continue;

	case 0x9f:
		illegal_1();
		continue;

	case 0xa0:
		illegal_1();
		continue;

	case 0xa1:
		illegal_1();
		continue;

	case 0xa2:
		illegal_1();
		continue;

	case 0xa3:
		illegal_1();
		continue;

	case 0xa4:
		and_a(HX);
		continue;

	case 0xa5:
		and_a(LX);
		continue;

	case 0xa6:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdda601;
			} else {
				m_ref = 0xdda602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdda603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdda604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdda605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdda606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdda607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdda608;
			} else {
				m_ref = 0xdda609;
			}
			return;
		}
		//case 0x09:
		and_a(TDAT8);
		continue;

	case 0xa7:
		illegal_1();
		continue;

	case 0xa8:
		illegal_1();
		continue;

	case 0xa9:
		illegal_1();
		continue;

	case 0xaa:
		illegal_1();
		continue;

	case 0xab:
		illegal_1();
		continue;

	case 0xac:
		xor_a(HX);
		continue;

	case 0xad:
		xor_a(LX);
		continue;

	case 0xae:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xddae01;
			} else {
				m_ref = 0xddae02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddae03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddae04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddae05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddae06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddae07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xddae08;
			} else {
				m_ref = 0xddae09;
			}
			return;
		}
		//case 0x09:
		xor_a(TDAT8);
		continue;

	case 0xaf:
		illegal_1();
		continue;

	case 0xb0:
		illegal_1();
		continue;

	case 0xb1:
		illegal_1();
		continue;

	case 0xb2:
		illegal_1();
		continue;

	case 0xb3:
		illegal_1();
		continue;

	case 0xb4:
		or_a(HX);
		continue;

	case 0xb5:
		or_a(LX);
		continue;

	case 0xb6:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xddb601;
			} else {
				m_ref = 0xddb602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddb603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddb604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddb605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddb606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddb607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xddb608;
			} else {
				m_ref = 0xddb609;
			}
			return;
		}
		//case 0x09:
		or_a(TDAT8);
		continue;

	case 0xb7:
		illegal_1();
		continue;

	case 0xb8:
		illegal_1();
		continue;

	case 0xb9:
		illegal_1();
		continue;

	case 0xba:
		illegal_1();
		continue;

	case 0xbb:
		illegal_1();
		continue;

	case 0xbc:
		cp(HX);
		continue;

	case 0xbd:
		cp(LX);
		continue;

	case 0xbe:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xddbe01;
			} else {
				m_ref = 0xddbe02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddbe03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddbe04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddbe05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddbe06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddbe07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xddbe08;
			} else {
				m_ref = 0xddbe09;
			}
			return;
		}
		//case 0x09:
		cp(TDAT8);
		continue;

	case 0xbf:
		illegal_1();
		continue;

	case 0xc0:
		illegal_1();
		continue;

	case 0xc1:
		illegal_1();
		continue;

	case 0xc2:
		illegal_1();
		continue;

	case 0xc3:
		illegal_1();
		continue;

	case 0xc4:
		illegal_1();
		continue;

	case 0xc5:
		illegal_1();
		continue;

	case 0xc6:
		illegal_1();
		continue;

	case 0xc7:
		illegal_1();
		continue;

	case 0xc8:
		illegal_1();
		continue;

	case 0xc9:
		illegal_1();
		continue;

	case 0xca:
		illegal_1();
		continue;

	case 0xcb:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xddcb01;
			} else {
				m_ref = 0xddcb02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IX + (s8)TDAT8);
		WZ = m_ea;
		//case 0x03:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xddcb03;
			} else {
				m_ref = 0xddcb04;
			}
			return;
		}
		//case 0x04:
		PC++;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddcb05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddcb06;
			return;
		}
		//case 0x06:
		m_ref = (0xfe << 16) | (TDAT8 << 8);
		goto process;
		continue;

	case 0xcc:
		illegal_1();
		continue;

	case 0xcd:
		illegal_1();
		continue;

	case 0xce:
		illegal_1();
		continue;

	case 0xcf:
		illegal_1();
		continue;

	case 0xd0:
		illegal_1();
		continue;

	case 0xd1:
		illegal_1();
		continue;

	case 0xd2:
		illegal_1();
		continue;

	case 0xd3:
		illegal_1();
		continue;

	case 0xd4:
		illegal_1();
		continue;

	case 0xd5:
		illegal_1();
		continue;

	case 0xd6:
		illegal_1();
		continue;

	case 0xd7:
		illegal_1();
		continue;

	case 0xd8:
		illegal_1();
		continue;

	case 0xd9:
		illegal_1();
		continue;

	case 0xda:
		illegal_1();
		continue;

	case 0xdb:
		illegal_1();
		continue;

	case 0xdc:
		illegal_1();
		continue;

	case 0xdd:
		illegal_1();
		continue;

	case 0xde:
		illegal_1();
		continue;

	case 0xdf:
		illegal_1();
		continue;

	case 0xe0:
		illegal_1();
		continue;

	case 0xe1:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdde101;
			} else {
				m_ref = 0xdde102;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdde103;
			} else {
				m_ref = 0xdde104;
			}
			return;
		}
		//case 0x04:
		SP++;
		IX = TDAT;
		continue;

	case 0xe2:
		illegal_1();
		continue;

	case 0xe3:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdde301;
			} else {
				m_ref = 0xdde302;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdde303;
			} else {
				m_ref = 0xdde304;
			}
			return;
		}
		//case 0x04:
		SP++;
		if (nomreq_en)
			m_nomreq_cb(SP-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdde305;
			return;
		}
		//case 0x05:
		SP--;
		//case 0x06:
		stack_write(SP, IX >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdde306;
			} else {
				m_ref = 0xdde307;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, IX);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdde308;
			} else {
				m_ref = 0xdde309;
			}
			return;
		}
		//case 0x09:
		IX = TDAT;
		if (nomreq_en)
			m_nomreq_cb(SP, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdde30a;
			return;
		}
		//case 0x0a:
		if (nomreq_en)
			m_nomreq_cb(SP, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdde30b;
			return;
		}
		//case 0x0b:
		WZ = TDAT;
		continue;

	case 0xe4:
		illegal_1();
		continue;

	case 0xe5:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xdde501;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, IX >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdde502;
			} else {
				m_ref = 0xdde503;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, IX);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xdde504;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xe6:
		illegal_1();
		continue;

	case 0xe7:
		illegal_1();
		continue;

	case 0xe8:
		illegal_1();
		continue;

	case 0xe9:
		PC = IX;
		continue;

	case 0xea:
		illegal_1();
		continue;

	case 0xeb:
		illegal_1();
		continue;

	case 0xec:
		illegal_1();
		continue;

	case 0xed:
		illegal_1();
		continue;

	case 0xee:
		illegal_1();
		continue;

	case 0xef:
		illegal_1();
		continue;

	case 0xf0:
		illegal_1();
		continue;

	case 0xf1:
		illegal_1();
		continue;

	case 0xf2:
		illegal_1();
		continue;

	case 0xf3:
		illegal_1();
		continue;

	case 0xf4:
		illegal_1();
		continue;

	case 0xf5:
		illegal_1();
		continue;

	case 0xf6:
		illegal_1();
		continue;

	case 0xf7:
		illegal_1();
		continue;

	case 0xf8:
		illegal_1();
		continue;

	case 0xf9:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddf901;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xddf902;
			return;
		}
		//case 0x02:
		SP = IX;
		continue;

	case 0xfa:
		illegal_1();
		continue;

	case 0xfb:
		illegal_1();
		continue;

	case 0xfc:
		illegal_1();
		continue;

	case 0xfd:
		illegal_1();
		continue;

	case 0xfe:
		illegal_1();
		continue;

	case 0xff:
		illegal_1();
		continue;

	}
} break; // prefix: dd

case 0xed:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x00:
		illegal_2();
		continue;

	case 0x01:
		illegal_2();
		continue;

	case 0x02:
		illegal_2();
		continue;

	case 0x03:
		illegal_2();
		continue;

	case 0x04:
		illegal_2();
		continue;

	case 0x05:
		illegal_2();
		continue;

	case 0x06:
		illegal_2();
		continue;

	case 0x07:
		illegal_2();
		continue;

	case 0x08:
		illegal_2();
		continue;

	case 0x09:
		illegal_2();
		continue;

	case 0x0a:
		illegal_2();
		continue;

	case 0x0b:
		illegal_2();
		continue;

	case 0x0c:
		illegal_2();
		continue;

	case 0x0d:
		illegal_2();
		continue;

	case 0x0e:
		illegal_2();
		continue;

	case 0x0f:
		illegal_2();
		continue;

	case 0x10:
		illegal_2();
		continue;

	case 0x11:
		illegal_2();
		continue;

	case 0x12:
		illegal_2();
		continue;

	case 0x13:
		illegal_2();
		continue;

	case 0x14:
		illegal_2();
		continue;

	case 0x15:
		illegal_2();
		continue;

	case 0x16:
		illegal_2();
		continue;

	case 0x17:
		illegal_2();
		continue;

	case 0x18:
		illegal_2();
		continue;

	case 0x19:
		illegal_2();
		continue;

	case 0x1a:
		illegal_2();
		continue;

	case 0x1b:
		illegal_2();
		continue;

	case 0x1c:
		illegal_2();
		continue;

	case 0x1d:
		illegal_2();
		continue;

	case 0x1e:
		illegal_2();
		continue;

	case 0x1f:
		illegal_2();
		continue;

	case 0x20:
		illegal_2();
		continue;

	case 0x21:
		illegal_2();
		continue;

	case 0x22:
		illegal_2();
		continue;

	case 0x23:
		illegal_2();
		continue;

	case 0x24:
		illegal_2();
		continue;

	case 0x25:
		illegal_2();
		continue;

	case 0x26:
		illegal_2();
		continue;

	case 0x27:
		illegal_2();
		continue;

	case 0x28:
		illegal_2();
		continue;

	case 0x29:
		illegal_2();
		continue;

	case 0x2a:
		illegal_2();
		continue;

	case 0x2b:
		illegal_2();
		continue;

	case 0x2c:
		illegal_2();
		continue;

	case 0x2d:
		illegal_2();
		continue;

	case 0x2e:
		illegal_2();
		continue;

	case 0x2f:
		illegal_2();
		continue;

	case 0x30:
		illegal_2();
		continue;

	case 0x31:
		illegal_2();
		continue;

	case 0x32:
		illegal_2();
		continue;

	case 0x33:
		illegal_2();
		continue;

	case 0x34:
		illegal_2();
		continue;

	case 0x35:
		illegal_2();
		continue;

	case 0x36:
		illegal_2();
		continue;

	case 0x37:
		illegal_2();
		continue;

	case 0x38:
		illegal_2();
		continue;

	case 0x39:
		illegal_2();
		continue;

	case 0x3a:
		illegal_2();
		continue;

	case 0x3b:
		illegal_2();
		continue;

	case 0x3c:
		illegal_2();
		continue;

	case 0x3d:
		illegal_2();
		continue;

	case 0x3e:
		illegal_2();
		continue;

	case 0x3f:
		illegal_2();
		continue;

	case 0x40:
		//case 0x00:
		//case 0x01:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed4001;
			} else {
				m_ref = 0xed4002;
			}
			return;
		}
		//case 0x02:
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
			m_f.yx_val = TDAT8;
			m_f.h_val = m_f.n = 0;
		}
		B = TDAT8;
		WZ = BC + 1;
		continue;

	case 0x41:
		//case 0x00:
		TDAT8 = B;
		//case 0x01:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed4101;
			} else {
				m_ref = 0xed4102;
			}
			return;
		}
		//case 0x02:
		WZ = BC + 1;
		continue;

	case 0x42:
		{
		u32 res = HL - BC - m_f.c;
		WZ = HL + 1;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = ((res & 0x8000) >> 8) | (u16(res) != 0);
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^ BC) >> 8;
			m_f.pv_val = !((BC ^ HL) & (HL ^ res) & 0x8000);
			m_f.n = 1;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x43:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4301;
			} else {
				m_ref = 0xed4302;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4303;
			} else {
				m_ref = 0xed4304;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		TDAT = BC;
		//case 0x05:
		data_write(m_ea, TDAT_L);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4305;
			} else {
				m_ref = 0xed4306;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		data_write(m_ea+1, TDAT_H);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4307;
			} else {
				m_ref = 0xed4308;
			}
			return;
		}
		//case 0x08:
		WZ = m_ea + 1;
		continue;

	case 0x44:
		neg();
		continue;

	case 0x45:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4501;
			} else {
				m_ref = 0xed4502;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4503;
			} else {
				m_ref = 0xed4504;
			}
			return;
		}
		//case 0x04:
		SP++;
		PC = TDAT;
		LOGMASKED(LOG_INT, "RETN m_iff1:%d m_iff2:%d\n", m_iff1, m_iff2);
		WZ = PC;
		m_iff1 = m_iff2;
		continue;

	case 0x46:
		//case 0x00:
		m_im = 0;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0x47:
		m_i = A;
		continue;

	case 0x48:
		//case 0x00:
		//case 0x01:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed4801;
			} else {
				m_ref = 0xed4802;
			}
			return;
		}
		//case 0x02:
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
			m_f.yx_val = TDAT8;
			m_f.h_val = m_f.n = 0;
		}
		C = TDAT8;
		WZ = BC + 1;
		continue;

	case 0x49:
		//case 0x00:
		TDAT8 = C;
		//case 0x01:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed4901;
			} else {
				m_ref = 0xed4902;
			}
			return;
		}
		//case 0x02:
		WZ = BC + 1;
		continue;

	case 0x4a:
		{
		u32 res = HL + BC + m_f.c;
		WZ = HL + 1;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = ((res & 0x8000) >> 8) | (u16(res) != 0);
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^ BC) >> 8;
			m_f.pv_val = !((BC ^ HL ^ 0x8000) & (BC ^ res) & 0x8000);
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x4b:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4b01;
			} else {
				m_ref = 0xed4b02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4b03;
			} else {
				m_ref = 0xed4b04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		//case 0x05:
		TDAT_L = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4b05;
			} else {
				m_ref = 0xed4b06;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		TDAT_H = data_read(m_ea+1);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4b07;
			} else {
				m_ref = 0xed4b08;
			}
			return;
		}
		//case 0x08:
		BC = TDAT;
		WZ = m_ea + 1;
		continue;

	case 0x4c:
		illegal_2();
		continue;

	case 0x4d:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4d01;
			} else {
				m_ref = 0xed4d02;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed4d03;
			} else {
				m_ref = 0xed4d04;
			}
			return;
		}
		//case 0x04:
		SP++;
		PC = TDAT;
		WZ = PC;
		m_iff1 = m_iff2;
		daisy_call_reti_device();
		continue;

	case 0x4e:
		illegal_2();
		continue;

	case 0x4f:
		m_r = A;
		m_r2 = A & 0x80; // keep bit 7 of r
		continue;

	case 0x50:
		//case 0x00:
		//case 0x01:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed5001;
			} else {
				m_ref = 0xed5002;
			}
			return;
		}
		//case 0x02:
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
			m_f.yx_val = TDAT8;
			m_f.h_val = m_f.n = 0;
		}
		D = TDAT8;
		WZ = BC + 1;
		continue;

	case 0x51:
		//case 0x00:
		TDAT8 = D;
		//case 0x01:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed5101;
			} else {
				m_ref = 0xed5102;
			}
			return;
		}
		//case 0x02:
		WZ = BC + 1;
		continue;

	case 0x52:
		{
		u32 res = HL - DE - m_f.c;
		WZ = HL + 1;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = ((res & 0x8000) >> 8) | (u16(res) != 0);
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^ DE) >> 8;
			m_f.pv_val = !((DE ^ HL) & (HL ^ res) & 0x8000);
			m_f.n = 1;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x53:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed5301;
			} else {
				m_ref = 0xed5302;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed5303;
			} else {
				m_ref = 0xed5304;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		TDAT = DE;
		//case 0x05:
		data_write(m_ea, TDAT_L);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed5305;
			} else {
				m_ref = 0xed5306;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		data_write(m_ea+1, TDAT_H);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed5307;
			} else {
				m_ref = 0xed5308;
			}
			return;
		}
		//case 0x08:
		WZ = m_ea + 1;
		continue;

	case 0x54:
		illegal_2();
		continue;

	case 0x55:
		illegal_2();
		continue;

	case 0x56:
		//case 0x00:
		m_im = 1;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0x57:
		A = m_i;
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = A;
			m_f.yx_val = A;
			m_f.h_val = m_f.n = 0;
			m_f.pv_val = !m_iff2;
		}
		if (HAS_LDAIR_QUIRK)
			set_service_attention<SA_AFTER_LDAIR, 1>();
		continue;

	case 0x58:
		//case 0x00:
		//case 0x01:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed5801;
			} else {
				m_ref = 0xed5802;
			}
			return;
		}
		//case 0x02:
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
			m_f.yx_val = TDAT8;
			m_f.h_val = m_f.n = 0;
		}
		E = TDAT8;
		WZ = BC + 1;
		continue;

	case 0x59:
		//case 0x00:
		TDAT8 = E;
		//case 0x01:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed5901;
			} else {
				m_ref = 0xed5902;
			}
			return;
		}
		//case 0x02:
		WZ = BC + 1;
		continue;

	case 0x5a:
		{
		u32 res = HL + DE + m_f.c;
		WZ = HL + 1;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = ((res & 0x8000) >> 8) | (u16(res) != 0);
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^ DE) >> 8;
			m_f.pv_val = !((DE ^ HL ^ 0x8000) & (DE ^ res) & 0x8000);
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x5b:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed5b01;
			} else {
				m_ref = 0xed5b02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed5b03;
			} else {
				m_ref = 0xed5b04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		//case 0x05:
		TDAT_L = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed5b05;
			} else {
				m_ref = 0xed5b06;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		TDAT_H = data_read(m_ea+1);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed5b07;
			} else {
				m_ref = 0xed5b08;
			}
			return;
		}
		//case 0x08:
		DE = TDAT;
		WZ = m_ea + 1;
		continue;

	case 0x5c:
		illegal_2();
		continue;

	case 0x5d:
		illegal_2();
		continue;

	case 0x5e:
		//case 0x00:
		m_im = 2;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0x5f:
		A = (m_r & 0x7f) | m_r2;
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = A;
			m_f.yx_val = A;
			m_f.h_val = m_f.n = 0;
			m_f.pv_val = !m_iff2;
		}
		if (HAS_LDAIR_QUIRK)
			set_service_attention<SA_AFTER_LDAIR, 1>();
		continue;

	case 0x60:
		//case 0x00:
		//case 0x01:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed6001;
			} else {
				m_ref = 0xed6002;
			}
			return;
		}
		//case 0x02:
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
			m_f.yx_val = TDAT8;
			m_f.h_val = m_f.n = 0;
		}
		H = TDAT8;
		WZ = BC + 1;
		continue;

	case 0x61:
		//case 0x00:
		TDAT8 = H;
		//case 0x01:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed6101;
			} else {
				m_ref = 0xed6102;
			}
			return;
		}
		//case 0x02:
		WZ = BC + 1;
		continue;

	case 0x62:
		{
		u32 res = HL - HL - m_f.c;
		WZ = HL + 1;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = ((res & 0x8000) >> 8) | (u16(res) != 0);
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^ HL) >> 8;
			m_f.pv_val = !((HL ^ HL) & (HL ^ res) & 0x8000);
			m_f.n = 1;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x63:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6301;
			} else {
				m_ref = 0xed6302;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6303;
			} else {
				m_ref = 0xed6304;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		TDAT = HL;
		//case 0x05:
		data_write(m_ea, TDAT_L);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6305;
			} else {
				m_ref = 0xed6306;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		data_write(m_ea+1, TDAT_H);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6307;
			} else {
				m_ref = 0xed6308;
			}
			return;
		}
		//case 0x08:
		WZ = m_ea + 1;
		continue;

	case 0x64:
		illegal_2();
		continue;

	case 0x65:
		illegal_2();
		continue;

	case 0x66:
		illegal_2();
		continue;

	case 0x67:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6701;
			} else {
				m_ref = 0xed6702;
			}
			return;
		}
		//case 0x02:
		WZ = HL+1;
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xed6703;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xed6704;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xed6705;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xed6706;
			return;
		}
		//case 0x06:
		TDAT_H = TDAT8;
		TDAT8 = (TDAT8 >> 4) | (A << 4);
		//case 0x07:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6707;
			} else {
				m_ref = 0xed6708;
			}
			return;
		}
		//case 0x08:
		A = (A & 0xf0) | (TDAT_H & 0x0f);
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = m_f.pv_val = A;
			m_f.yx_val = A;
			m_f.h_val = m_f.n = 0;
		}
		continue;

	case 0x68:
		//case 0x00:
		//case 0x01:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed6801;
			} else {
				m_ref = 0xed6802;
			}
			return;
		}
		//case 0x02:
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
			m_f.yx_val = TDAT8;
			m_f.h_val = m_f.n = 0;
		}
		L = TDAT8;
		WZ = BC + 1;
		continue;

	case 0x69:
		//case 0x00:
		TDAT8 = L;
		//case 0x01:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed6901;
			} else {
				m_ref = 0xed6902;
			}
			return;
		}
		//case 0x02:
		WZ = BC + 1;
		continue;

	case 0x6a:
		{
		u32 res = HL + HL + m_f.c;
		WZ = HL + 1;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = ((res & 0x8000) >> 8) | (u16(res) != 0);
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^ HL) >> 8;
			m_f.pv_val = !((HL ^ HL ^ 0x8000) & (HL ^ res) & 0x8000);
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x6b:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6b01;
			} else {
				m_ref = 0xed6b02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6b03;
			} else {
				m_ref = 0xed6b04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		//case 0x05:
		TDAT_L = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6b05;
			} else {
				m_ref = 0xed6b06;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		TDAT_H = data_read(m_ea+1);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6b07;
			} else {
				m_ref = 0xed6b08;
			}
			return;
		}
		//case 0x08:
		HL = TDAT;
		WZ = m_ea + 1;
		continue;

	case 0x6c:
		illegal_2();
		continue;

	case 0x6d:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6d01;
			} else {
				m_ref = 0xed6d02;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6d03;
			} else {
				m_ref = 0xed6d04;
			}
			return;
		}
		//case 0x04:
		SP++;
		PC = TDAT;
		WZ = PC;
		m_iff1 = m_iff2;
		daisy_call_reti_device();
		continue;

	case 0x6e:
		//case 0x00:
		m_im = 0;
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0x6f:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6f01;
			} else {
				m_ref = 0xed6f02;
			}
			return;
		}
		//case 0x02:
		WZ = HL+1;
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xed6f03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xed6f04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xed6f05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xed6f06;
			return;
		}
		//case 0x06:
		TDAT_H = TDAT8;
		TDAT8 = (TDAT8 << 4) | (A & 0x0f);
		//case 0x07:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed6f07;
			} else {
				m_ref = 0xed6f08;
			}
			return;
		}
		//case 0x08:
		A = (A & 0xf0) | (TDAT_H >> 4);
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = m_f.pv_val = A;
			m_f.yx_val = A;
			m_f.h_val = m_f.n = 0;
		}
		continue;

	case 0x70:
		//case 0x00:
		//case 0x01:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed7001;
			} else {
				m_ref = 0xed7002;
			}
			return;
		}
		//case 0x02:
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
			m_f.yx_val = TDAT8;
			m_f.h_val = m_f.n = 0;
		}
		WZ = BC + 1;
		continue;

	case 0x71:
		illegal_2();
		continue;

	case 0x72:
		{
		u32 res = HL - SP - m_f.c;
		WZ = HL + 1;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = ((res & 0x8000) >> 8) | (u16(res) != 0);
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^ SP) >> 8;
			m_f.pv_val = !((SP ^ HL) & (HL ^ res) & 0x8000);
			m_f.n = 1;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x73:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed7301;
			} else {
				m_ref = 0xed7302;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed7303;
			} else {
				m_ref = 0xed7304;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		TDAT = SP;
		//case 0x05:
		data_write(m_ea, TDAT_L);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed7305;
			} else {
				m_ref = 0xed7306;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		data_write(m_ea+1, TDAT_H);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed7307;
			} else {
				m_ref = 0xed7308;
			}
			return;
		}
		//case 0x08:
		WZ = m_ea + 1;
		continue;

	case 0x74:
		illegal_2();
		continue;

	case 0x75:
		illegal_2();
		continue;

	case 0x76:
		illegal_2();
		continue;

	case 0x77:
		illegal_2();
		continue;

	case 0x78:
		//case 0x00:
		//case 0x01:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed7801;
			} else {
				m_ref = 0xed7802;
			}
			return;
		}
		//case 0x02:
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = m_f.pv_val = TDAT8;
			m_f.yx_val = TDAT8;
			m_f.h_val = m_f.n = 0;
		}
		A = TDAT8;
		WZ = BC + 1;
		continue;

	case 0x79:
		//case 0x00:
		TDAT8 = A;
		//case 0x01:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xed7901;
			} else {
				m_ref = 0xed7902;
			}
			return;
		}
		//case 0x02:
		WZ = BC + 1;
		continue;

	case 0x7a:
		{
		u32 res = HL + SP + m_f.c;
		WZ = HL + 1;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = ((res & 0x8000) >> 8) | (u16(res) != 0);
			m_f.yx_val = res >> 8;
			m_f.h_val = (HL ^ res ^ SP) >> 8;
			m_f.pv_val = !((SP ^ HL ^ 0x8000) & (SP ^ res) & 0x8000);
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		HL = (u16)res;
		}
		continue;

	case 0x7b:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed7b01;
			} else {
				m_ref = 0xed7b02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed7b03;
			} else {
				m_ref = 0xed7b04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		//case 0x05:
		TDAT_L = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed7b05;
			} else {
				m_ref = 0xed7b06;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		TDAT_H = data_read(m_ea+1);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xed7b07;
			} else {
				m_ref = 0xed7b08;
			}
			return;
		}
		//case 0x08:
		SP = TDAT;
		WZ = m_ea + 1;
		continue;

	case 0x7c:
		illegal_2();
		continue;

	case 0x7d:
		illegal_2();
		continue;

	case 0x7e:
		illegal_2();
		continue;

	case 0x7f:
		illegal_2();
		continue;

	case 0x80:
		illegal_2();
		continue;

	case 0x81:
		illegal_2();
		continue;

	case 0x82:
		illegal_2();
		continue;

	case 0x83:
		illegal_2();
		continue;

	case 0x84:
		illegal_2();
		continue;

	case 0x85:
		illegal_2();
		continue;

	case 0x86:
		illegal_2();
		continue;

	case 0x87:
		illegal_2();
		continue;

	case 0x88:
		illegal_2();
		continue;

	case 0x89:
		illegal_2();
		continue;

	case 0x8a:
		illegal_2();
		continue;

	case 0x8b:
		illegal_2();
		continue;

	case 0x8c:
		illegal_2();
		continue;

	case 0x8d:
		illegal_2();
		continue;

	case 0x8e:
		illegal_2();
		continue;

	case 0x8f:
		illegal_2();
		continue;

	case 0x90:
		illegal_2();
		continue;

	case 0x91:
		illegal_2();
		continue;

	case 0x92:
		illegal_2();
		continue;

	case 0x93:
		illegal_2();
		continue;

	case 0x94:
		illegal_2();
		continue;

	case 0x95:
		illegal_2();
		continue;

	case 0x96:
		illegal_2();
		continue;

	case 0x97:
		illegal_2();
		continue;

	case 0x98:
		illegal_2();
		continue;

	case 0x99:
		illegal_2();
		continue;

	case 0x9a:
		illegal_2();
		continue;

	case 0x9b:
		illegal_2();
		continue;

	case 0x9c:
		illegal_2();
		continue;

	case 0x9d:
		illegal_2();
		continue;

	case 0x9e:
		illegal_2();
		continue;

	case 0x9f:
		illegal_2();
		continue;

	case 0xa0:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xeda001;
			} else {
				m_ref = 0xeda002;
			}
			return;
		}
		//case 0x02:
		//case 0x03:
		data_write(DE, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xeda003;
			} else {
				m_ref = 0xeda004;
			}
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(DE, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xeda005;
			return;
		}
		//case 0x05:
		HL++;
		DE++;
		BC--;
		{
			QT = 0;
			// keep SZC
			m_f.yx_val = ((A + TDAT8) << 4) | ((A + TDAT8) & 0x0f);
			m_f.h_val = m_f.n = 0;
			m_f.pv_val = !BC;
		}
		continue;

	case 0xa1:
		//case 0x00:
		{
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xeda101;
			} else {
				m_ref = 0xeda102;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xeda103;
			return;
		}
		//case 0x03:
		WZ++;
		HL++;
		BC--;
		u8 res = A - TDAT8;
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = res;
			m_f.h_val = A ^ TDAT8 ^ res;
			if (m_f.h())
				res -= 1;
			m_f.yx_val = (res << 4) | (res & 0x0f);
			m_f.pv_val = !BC;
			m_f.n = 1;
		}
		}
		continue;

	case 0xa2:
		//case 0x00:
		{
		//case 0x01:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xeda201;
			} else {
				m_ref = 0xeda202;
			}
			return;
		}
		//case 0x02:
		WZ = BC + 1;
		B--;
		//case 0x03:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xeda203;
			} else {
				m_ref = 0xeda204;
			}
			return;
		}
		//case 0x04:
		HL++;
		const u16 t = ((C + 1) & 0xff) + TDAT8;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = B;
			m_f.yx_val = B;
			m_f.h_val = t >> 4;
			m_f.pv_val = (t & 0x07) ^ B;
			m_f.n = TDAT8 & SF;
			m_f.c = t & 0x100;
		}
		}
		continue;

	case 0xa3:
		//case 0x00:
		{
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xeda301;
			} else {
				m_ref = 0xeda302;
			}
			return;
		}
		//case 0x02:
		B--;
		WZ = BC + 1;
		//case 0x03:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xeda303;
			} else {
				m_ref = 0xeda304;
			}
			return;
		}
		//case 0x04:
		HL++;
		const u16 t = L + TDAT8;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = B;
			m_f.yx_val = B;
			m_f.h_val = t >> 4;
			m_f.pv_val = (t & 0x07) ^ B;
			m_f.n = TDAT8 & SF;
			m_f.c = t & 0x100;
		}
		}
		continue;

	case 0xa4:
		illegal_2();
		continue;

	case 0xa5:
		illegal_2();
		continue;

	case 0xa6:
		illegal_2();
		continue;

	case 0xa7:
		illegal_2();
		continue;

	case 0xa8:
		//case 0x00:
		{
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xeda801;
			} else {
				m_ref = 0xeda802;
			}
			return;
		}
		//case 0x02:
		//case 0x03:
		data_write(DE, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xeda803;
			} else {
				m_ref = 0xeda804;
			}
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(DE, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xeda805;
			return;
		}
		//case 0x05:
		HL--;
		DE--;
		BC--;
		{
			QT = 0;
			// keep SZC
			m_f.yx_val = ((A + TDAT8) << 4) | ((A + TDAT8) & 0x0f);
			m_f.h_val = m_f.n = 0;
			m_f.pv_val = !BC;
		}
		}
		continue;

	case 0xa9:
		//case 0x00:
		{
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xeda901;
			} else {
				m_ref = 0xeda902;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xeda903;
			return;
		}
		//case 0x03:
		WZ--;
		HL--;
		BC--;
		u8 res = A - TDAT8;
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = res;
			m_f.h_val = A ^ TDAT8 ^ res;
			if (m_f.h())
				res -= 1;
			m_f.yx_val = (res << 4) | (res & 0x0f);
			m_f.pv_val = !BC;
			m_f.n = 1;
		}
		}
		continue;

	case 0xaa:
		//case 0x00:
		{
		//case 0x01:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xedaa01;
			} else {
				m_ref = 0xedaa02;
			}
			return;
		}
		//case 0x02:
		WZ = BC - 1;
		B--;
		//case 0x03:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedaa03;
			} else {
				m_ref = 0xedaa04;
			}
			return;
		}
		//case 0x04:
		HL--;
		const u16 t = ((C - 1) & 0xff) + TDAT8;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = B;
			m_f.yx_val = B;
			m_f.h_val = t >> 4;
			m_f.pv_val = (t & 0x07) ^ B;
			m_f.n = TDAT8 & SF;
			m_f.c = t & 0x100;
		}
		}
		continue;

	case 0xab:
		//case 0x00:
		{
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedab01;
			} else {
				m_ref = 0xedab02;
			}
			return;
		}
		//case 0x02:
		B--;
		WZ = BC - 1;
		//case 0x03:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xedab03;
			} else {
				m_ref = 0xedab04;
			}
			return;
		}
		//case 0x04:
		HL--;
		const u16 t = L + TDAT8;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = B;
			m_f.yx_val = B;
			m_f.h_val = t >> 4;
			m_f.pv_val = (t & 0x07) ^ B;
			m_f.n = TDAT8 & SF;
			m_f.c = t & 0x100;
		}
		}
		continue;

	case 0xac:
		illegal_2();
		continue;

	case 0xad:
		illegal_2();
		continue;

	case 0xae:
		illegal_2();
		continue;

	case 0xaf:
		illegal_2();
		continue;

	case 0xb0:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedb001;
			} else {
				m_ref = 0xedb002;
			}
			return;
		}
		//case 0x02:
		//case 0x03:
		data_write(DE, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedb003;
			} else {
				m_ref = 0xedb004;
			}
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(DE, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb005;
			return;
		}
		//case 0x05:
		HL++;
		DE++;
		BC--;
		{
			QT = 0;
			// keep SZC
			m_f.yx_val = ((A + TDAT8) << 4) | ((A + TDAT8) & 0x0f);
			m_f.h_val = m_f.n = 0;
			m_f.pv_val = !BC;
		}
		if (BC != 0) {
		if (nomreq_en)
			m_nomreq_cb(DE, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb006;
			return;
		}
		//case 0x06:
			PC -= 2;
			WZ = PC + 1;
			m_f.yx_val = PC >> 8;
		}
		continue;

	case 0xb1:
		//case 0x00:
		{
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedb101;
			} else {
				m_ref = 0xedb102;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb103;
			return;
		}
		//case 0x03:
		WZ++;
		HL++;
		BC--;
		u8 res = A - TDAT8;
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = res;
			m_f.h_val = A ^ TDAT8 ^ res;
			if (m_f.h())
				res -= 1;
			m_f.yx_val = (res << 4) | (res & 0x0f);
			m_f.pv_val = !BC;
			m_f.n = 1;
		}
		}
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb104;
			return;
		}
		//case 0x04:
		if (BC != 0 && !m_f.z()) {
			PC -= 2;
			WZ = PC + 1;
			m_f.yx_val = PC >> 8;
		}
		continue;

	case 0xb2:
		//case 0x00:
		if (B == 1) {
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb201;
				return;
			}
		//case 0x01:
			;
		}
		{
		//case 0x02:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xedb202;
			} else {
				m_ref = 0xedb203;
			}
			return;
		}
		//case 0x03:
		WZ = BC + 1;
		B--;
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedb204;
			} else {
				m_ref = 0xedb205;
			}
			return;
		}
		//case 0x05:
		HL++;
		const u16 t = ((C + 1) & 0xff) + TDAT8;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = B;
			m_f.yx_val = B;
			m_f.h_val = t >> 4;
			m_f.pv_val = (t & 0x07) ^ B;
			m_f.n = TDAT8 & SF;
			m_f.c = t & 0x100;
		}
		}
		if (B != 0) {
			PC -= 2;
			block_io_interrupted_flags();
		}
		continue;

	case 0xb3:
		//case 0x00:
		if (B == 1) {
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedb301;
				return;
			}
		//case 0x01:
			;
		}
		{
		//case 0x02:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedb302;
			} else {
				m_ref = 0xedb303;
			}
			return;
		}
		//case 0x03:
		B--;
		WZ = BC + 1;
		//case 0x04:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xedb304;
			} else {
				m_ref = 0xedb305;
			}
			return;
		}
		//case 0x05:
		HL++;
		const u16 t = L + TDAT8;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = B;
			m_f.yx_val = B;
			m_f.h_val = t >> 4;
			m_f.pv_val = (t & 0x07) ^ B;
			m_f.n = TDAT8 & SF;
			m_f.c = t & 0x100;
		}
		}
		if (B != 0) {
		if (nomreq_en)
			m_nomreq_cb(BC, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb306;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(BC, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb307;
			return;
		}
		//case 0x07:
		if (nomreq_en)
			m_nomreq_cb(BC, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb308;
			return;
		}
		//case 0x08:
		if (nomreq_en)
			m_nomreq_cb(BC, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb309;
			return;
		}
		//case 0x09:
		if (nomreq_en)
			m_nomreq_cb(BC, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb30a;
			return;
		}
		//case 0x0a:
			PC -= 2;
			block_io_interrupted_flags();
		}
		continue;

	case 0xb4:
		illegal_2();
		continue;

	case 0xb5:
		illegal_2();
		continue;

	case 0xb6:
		illegal_2();
		continue;

	case 0xb7:
		illegal_2();
		continue;

	case 0xb8:
		//case 0x00:
		{
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedb801;
			} else {
				m_ref = 0xedb802;
			}
			return;
		}
		//case 0x02:
		//case 0x03:
		data_write(DE, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedb803;
			} else {
				m_ref = 0xedb804;
			}
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(DE, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb805;
			return;
		}
		//case 0x05:
		HL--;
		DE--;
		BC--;
		{
			QT = 0;
			// keep SZC
			m_f.yx_val = ((A + TDAT8) << 4) | ((A + TDAT8) & 0x0f);
			m_f.h_val = m_f.n = 0;
			m_f.pv_val = !BC;
		}
		}
		if (BC != 0) {
		if (nomreq_en)
			m_nomreq_cb(DE, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb806;
			return;
		}
		//case 0x06:
			PC -= 2;
			WZ = PC + 1;
			m_f.yx_val = PC >> 8;
		}
		continue;

	case 0xb9:
		//case 0x00:
		{
		//case 0x01:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedb901;
			} else {
				m_ref = 0xedb902;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(HL, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb903;
			return;
		}
		//case 0x03:
		WZ--;
		HL--;
		BC--;
		u8 res = A - TDAT8;
		{
			QT = 0;
			// keep C
			m_f.s_val = m_f.z_val = res;
			m_f.h_val = A ^ TDAT8 ^ res;
			if (m_f.h())
				res -= 1;
			m_f.yx_val = (res << 4) | (res & 0x0f);
			m_f.pv_val = !BC;
			m_f.n = 1;
		}
		}
		if (nomreq_en)
			m_nomreq_cb(HL /* suspected wrong. must be inside if? */, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xedb904;
			return;
		}
		//case 0x04:
		if (BC != 0 && !m_f.z()) {
			PC -= 2;
			WZ = PC + 1;
			m_f.yx_val = PC >> 8;
		}
		continue;

	case 0xba:
		//case 0x00:
		if (B == 1) {
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedba01;
				return;
			}
		//case 0x01:
			;
		}
		{
		//case 0x02:
		TDAT8 = m_io.read_interruptible(BC);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xedba02;
			} else {
				m_ref = 0xedba03;
			}
			return;
		}
		//case 0x03:
		WZ = BC - 1;
		B--;
		//case 0x04:
		data_write(HL, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedba04;
			} else {
				m_ref = 0xedba05;
			}
			return;
		}
		//case 0x05:
		HL--;
		const u16 t = ((C - 1) & 0xff) + TDAT8;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = B;
			m_f.yx_val = B;
			m_f.h_val = t >> 4;
			m_f.pv_val = (t & 0x07) ^ B;
			m_f.n = TDAT8 & SF;
			m_f.c = t & 0x100;
		}
		}
		if (B != 0) {
			PC -= 2;
			block_io_interrupted_flags();
		}
		continue;

	case 0xbb:
		//case 0x00:
		if (B == 1) {
			m_icount -= 1;
			if (m_icount <= 0) {
				m_ref = 0xedbb01;
				return;
			}
		//case 0x01:
			;
		}
		{
		//case 0x02:
		TDAT8 = data_read(HL);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xedbb02;
			} else {
				m_ref = 0xedbb03;
			}
			return;
		}
		//case 0x03:
		B--;
		WZ = BC - 1;
		//case 0x04:
		m_io.write_interruptible(BC, TDAT8);
		m_icount -= m_iorq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_iorq_cycles;
				m_ref = 0xedbb04;
			} else {
				m_ref = 0xedbb05;
			}
			return;
		}
		//case 0x05:
		HL--;
		const u16 t = L + TDAT8;
		{
			QT = 0;
			m_f.s_val = m_f.z_val = B;
			m_f.yx_val = B;
			m_f.h_val = t >> 4;
			m_f.pv_val = (t & 0x07) ^ B;
			m_f.n = TDAT8 & SF;
			m_f.c = t & 0x100;
		}
		}
		if (B != 0) {
			PC -= 2;
			block_io_interrupted_flags();
		}
		continue;

	case 0xbc:
		illegal_2();
		continue;

	case 0xbd:
		illegal_2();
		continue;

	case 0xbe:
		illegal_2();
		continue;

	case 0xbf:
		illegal_2();
		continue;

	case 0xc0:
		illegal_2();
		continue;

	case 0xc1:
		//case 0x00:
		mulub(B);
		m_icount -= 12;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xc2:
		illegal_2();
		continue;

	case 0xc3:
		//case 0x00:
		muluw(BC);
		m_icount -= 34;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xc4:
		illegal_2();
		continue;

	case 0xc5:
		illegal_2();
		continue;

	case 0xc6:
		illegal_2();
		continue;

	case 0xc7:
		illegal_2();
		continue;

	case 0xc8:
		illegal_2();
		continue;

	case 0xc9:
		//case 0x00:
		mulub(C);
		m_icount -= 12;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xca:
		illegal_2();
		continue;

	case 0xcb:
		illegal_2();
		continue;

	case 0xcc:
		illegal_2();
		continue;

	case 0xcd:
		illegal_2();
		continue;

	case 0xce:
		illegal_2();
		continue;

	case 0xcf:
		illegal_2();
		continue;

	case 0xd0:
		illegal_2();
		continue;

	case 0xd1:
		//case 0x00:
		mulub(D);
		m_icount -= 12;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xd2:
		illegal_2();
		continue;

	case 0xd3:
		illegal_2();
		continue;

	case 0xd4:
		illegal_2();
		continue;

	case 0xd5:
		illegal_2();
		continue;

	case 0xd6:
		illegal_2();
		continue;

	case 0xd7:
		illegal_2();
		continue;

	case 0xd8:
		illegal_2();
		continue;

	case 0xd9:
		//case 0x00:
		mulub(E);
		m_icount -= 12;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xda:
		illegal_2();
		continue;

	case 0xdb:
		illegal_2();
		continue;

	case 0xdc:
		illegal_2();
		continue;

	case 0xdd:
		illegal_2();
		continue;

	case 0xde:
		illegal_2();
		continue;

	case 0xdf:
		illegal_2();
		continue;

	case 0xe0:
		illegal_2();
		continue;

	case 0xe1:
		//case 0x00:
		mulub(H);
		m_icount -= 12;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xe2:
		illegal_2();
		continue;

	case 0xe3:
		illegal_2();
		continue;

	case 0xe4:
		illegal_2();
		continue;

	case 0xe5:
		illegal_2();
		continue;

	case 0xe6:
		illegal_2();
		continue;

	case 0xe7:
		illegal_2();
		continue;

	case 0xe8:
		illegal_2();
		continue;

	case 0xe9:
		//case 0x00:
		mulub(L);
		m_icount -= 12;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xea:
		illegal_2();
		continue;

	case 0xeb:
		illegal_2();
		continue;

	case 0xec:
		illegal_2();
		continue;

	case 0xed:
		illegal_2();
		continue;

	case 0xee:
		illegal_2();
		continue;

	case 0xef:
		illegal_2();
		continue;

	case 0xf0:
		illegal_2();
		continue;

	case 0xf1:
		//case 0x00:
		mulub(A);
		m_icount -= 12;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xf2:
		illegal_2();
		continue;

	case 0xf3:
		//case 0x00:
		muluw(SP);
		m_icount -= 34;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xf4:
		illegal_2();
		continue;

	case 0xf5:
		illegal_2();
		continue;

	case 0xf6:
		illegal_2();
		continue;

	case 0xf7:
		illegal_2();
		continue;

	case 0xf8:
		illegal_2();
		continue;

	case 0xf9:
		//case 0x00:
		mulub(A);
		m_icount -= 12;
		if (m_icount <= 0) {
			m_ref = 0xffff00;
			return;
		}
		continue;

	case 0xfa:
		illegal_2();
		continue;

	case 0xfb:
		illegal_2();
		continue;

	case 0xfc:
		illegal_2();
		continue;

	case 0xfd:
		illegal_2();
		continue;

	case 0xfe:
		illegal_2();
		continue;

	case 0xff:
		illegal_2();
		continue;

	}
} break; // prefix: ed

case 0xfd:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x00:
		illegal_1();
		continue;

	case 0x01:
		illegal_1();
		continue;

	case 0x02:
		illegal_1();
		continue;

	case 0x03:
		illegal_1();
		continue;

	case 0x04:
		illegal_1();
		continue;

	case 0x05:
		illegal_1();
		continue;

	case 0x06:
		illegal_1();
		continue;

	case 0x07:
		illegal_1();
		continue;

	case 0x08:
		illegal_1();
		continue;

	case 0x09:
		{
		const u32 res = IY +  BC;
		WZ = IY + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (IY ^ res ^  BC) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		IY = (u16)res;
		}
		continue;

	case 0x0a:
		illegal_1();
		continue;

	case 0x0b:
		illegal_1();
		continue;

	case 0x0c:
		illegal_1();
		continue;

	case 0x0d:
		illegal_1();
		continue;

	case 0x0e:
		illegal_1();
		continue;

	case 0x0f:
		illegal_1();
		continue;

	case 0x10:
		illegal_1();
		continue;

	case 0x11:
		illegal_1();
		continue;

	case 0x12:
		illegal_1();
		continue;

	case 0x13:
		illegal_1();
		continue;

	case 0x14:
		illegal_1();
		continue;

	case 0x15:
		illegal_1();
		continue;

	case 0x16:
		illegal_1();
		continue;

	case 0x17:
		illegal_1();
		continue;

	case 0x18:
		illegal_1();
		continue;

	case 0x19:
		{
		const u32 res = IY +  DE;
		WZ = IY + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (IY ^ res ^  DE) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		IY = (u16)res;
		}
		continue;

	case 0x1a:
		illegal_1();
		continue;

	case 0x1b:
		illegal_1();
		continue;

	case 0x1c:
		illegal_1();
		continue;

	case 0x1d:
		illegal_1();
		continue;

	case 0x1e:
		illegal_1();
		continue;

	case 0x1f:
		illegal_1();
		continue;

	case 0x20:
		illegal_1();
		continue;

	case 0x21:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2101;
			} else {
				m_ref = 0xfd2102;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2103;
			} else {
				m_ref = 0xfd2104;
			}
			return;
		}
		//case 0x04:
		PC++;
		IY = TDAT;
		continue;

	case 0x22:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2201;
			} else {
				m_ref = 0xfd2202;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2203;
			} else {
				m_ref = 0xfd2204;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		TDAT = IY;
		//case 0x05:
		data_write(m_ea, TDAT_L);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2205;
			} else {
				m_ref = 0xfd2206;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		data_write(m_ea+1, TDAT_H);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2207;
			} else {
				m_ref = 0xfd2208;
			}
			return;
		}
		//case 0x08:
		WZ = m_ea + 1;
		continue;

	case 0x23:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd2301;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd2302;
			return;
		}
		//case 0x02:
		IY++;
		continue;

	case 0x24:
		inc(HY);
		continue;

	case 0x25:
		dec(HY);
		continue;

	case 0x26:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2601;
			} else {
				m_ref = 0xfd2602;
			}
			return;
		}
		//case 0x02:
		PC++;
		HY = TDAT8;
		continue;

	case 0x27:
		illegal_1();
		continue;

	case 0x28:
		illegal_1();
		continue;

	case 0x29:
		{
		const u32 res = IY +  IY;
		WZ = IY + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (IY ^ res ^  IY) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		IY = (u16)res;
		}
		continue;

	case 0x2a:
		//case 0x00:
		//case 0x01:
		TDAT_L = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2a01;
			} else {
				m_ref = 0xfd2a02;
			}
			return;
		}
		//case 0x02:
		PC++;
		//case 0x03:
		TDAT_H = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2a03;
			} else {
				m_ref = 0xfd2a04;
			}
			return;
		}
		//case 0x04:
		PC++;
		m_ea = TDAT;
		//case 0x05:
		TDAT_L = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2a05;
			} else {
				m_ref = 0xfd2a06;
			}
			return;
		}
		//case 0x06:
		//case 0x07:
		TDAT_H = data_read(m_ea+1);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2a07;
			} else {
				m_ref = 0xfd2a08;
			}
			return;
		}
		//case 0x08:
		IY = TDAT;
		WZ = m_ea + 1;
		continue;

	case 0x2b:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd2b01;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd2b02;
			return;
		}
		//case 0x02:
		IY--;
		continue;

	case 0x2c:
		inc(LY);
		continue;

	case 0x2d:
		dec(LY);
		continue;

	case 0x2e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd2e01;
			} else {
				m_ref = 0xfd2e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		LY = TDAT8;
		continue;

	case 0x2f:
		illegal_1();
		continue;

	case 0x30:
		illegal_1();
		continue;

	case 0x31:
		illegal_1();
		continue;

	case 0x32:
		illegal_1();
		continue;

	case 0x33:
		illegal_1();
		continue;

	case 0x34:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd3401;
			} else {
				m_ref = 0xfd3402;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3403;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3404;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3405;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3406;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3407;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd3408;
			} else {
				m_ref = 0xfd3409;
			}
			return;
		}
		//case 0x09:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd340a;
			return;
		}
		//case 0x0a:
		inc(TDAT8);
		//case 0x0b:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd340b;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x35:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd3501;
			} else {
				m_ref = 0xfd3502;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3503;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3504;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3505;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3506;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3507;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd3508;
			} else {
				m_ref = 0xfd3509;
			}
			return;
		}
		//case 0x09:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd350a;
			return;
		}
		//case 0x0a:
		dec(TDAT8);
		//case 0x0b:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd350b;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x36:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd3601;
			} else {
				m_ref = 0xfd3602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		//case 0x03:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd3603;
			} else {
				m_ref = 0xfd3604;
			}
			return;
		}
		//case 0x04:
		PC++;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd3606;
			return;
		}
		//case 0x06:
		//case 0x07:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd3607;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x37:
		illegal_1();
		continue;

	case 0x38:
		illegal_1();
		continue;

	case 0x39:
		{
		const u32 res = IY +  SP;
		WZ = IY + 1;
		{
			QT = 0;
			// keep szv
			m_f.yx_val = res >> 8;
			m_f.h_val = (IY ^ res ^  SP) >> 8;
			m_f.n = 0;
			m_f.c = res & 0x10000;
		}
		IY = (u16)res;
		}
		continue;

	case 0x3a:
		illegal_1();
		continue;

	case 0x3b:
		illegal_1();
		continue;

	case 0x3c:
		illegal_1();
		continue;

	case 0x3d:
		illegal_1();
		continue;

	case 0x3e:
		illegal_1();
		continue;

	case 0x3f:
		illegal_1();
		continue;

	case 0x40:
		illegal_1();
		continue;

	case 0x41:
		illegal_1();
		continue;

	case 0x42:
		illegal_1();
		continue;

	case 0x43:
		illegal_1();
		continue;

	case 0x44:
		B = HY;
		continue;

	case 0x45:
		B = LY;
		continue;

	case 0x46:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd4601;
			} else {
				m_ref = 0xfd4602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd4603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd4604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd4605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd4606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd4607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd4608;
			} else {
				m_ref = 0xfd4609;
			}
			return;
		}
		//case 0x09:
		B = TDAT8;
		continue;

	case 0x47:
		illegal_1();
		continue;

	case 0x48:
		illegal_1();
		continue;

	case 0x49:
		illegal_1();
		continue;

	case 0x4a:
		illegal_1();
		continue;

	case 0x4b:
		illegal_1();
		continue;

	case 0x4c:
		C = HY;
		continue;

	case 0x4d:
		C = LY;
		continue;

	case 0x4e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd4e01;
			} else {
				m_ref = 0xfd4e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd4e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd4e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd4e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd4e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd4e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd4e08;
			} else {
				m_ref = 0xfd4e09;
			}
			return;
		}
		//case 0x09:
		C = TDAT8;
		continue;

	case 0x4f:
		illegal_1();
		continue;

	case 0x50:
		illegal_1();
		continue;

	case 0x51:
		illegal_1();
		continue;

	case 0x52:
		illegal_1();
		continue;

	case 0x53:
		illegal_1();
		continue;

	case 0x54:
		D = HY;
		continue;

	case 0x55:
		D = LY;
		continue;

	case 0x56:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd5601;
			} else {
				m_ref = 0xfd5602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd5603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd5604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd5605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd5606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd5607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd5608;
			} else {
				m_ref = 0xfd5609;
			}
			return;
		}
		//case 0x09:
		D = TDAT8;
		continue;

	case 0x57:
		illegal_1();
		continue;

	case 0x58:
		illegal_1();
		continue;

	case 0x59:
		illegal_1();
		continue;

	case 0x5a:
		illegal_1();
		continue;

	case 0x5b:
		illegal_1();
		continue;

	case 0x5c:
		E = HY;
		continue;

	case 0x5d:
		E = LY;
		continue;

	case 0x5e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd5e01;
			} else {
				m_ref = 0xfd5e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd5e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd5e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd5e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd5e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd5e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd5e08;
			} else {
				m_ref = 0xfd5e09;
			}
			return;
		}
		//case 0x09:
		E = TDAT8;
		continue;

	case 0x5f:
		illegal_1();
		continue;

	case 0x60:
		HY = B;
		continue;

	case 0x61:
		HY = C;
		continue;

	case 0x62:
		HY = D;
		continue;

	case 0x63:
		HY = E;
		continue;

	case 0x64:
		continue;

	case 0x65:
		HY = LY;
		continue;

	case 0x66:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd6601;
			} else {
				m_ref = 0xfd6602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd6603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd6604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd6605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd6606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd6607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd6608;
			} else {
				m_ref = 0xfd6609;
			}
			return;
		}
		//case 0x09:
		H = TDAT8;
		continue;

	case 0x67:
		HY = A;
		continue;

	case 0x68:
		LY = B;
		continue;

	case 0x69:
		LY = C;
		continue;

	case 0x6a:
		LY = D;
		continue;

	case 0x6b:
		LY = E;
		continue;

	case 0x6c:
		LY = HY;
		continue;

	case 0x6d:
		continue;

	case 0x6e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd6e01;
			} else {
				m_ref = 0xfd6e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd6e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd6e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd6e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd6e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd6e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd6e08;
			} else {
				m_ref = 0xfd6e09;
			}
			return;
		}
		//case 0x09:
		L = TDAT8;
		continue;

	case 0x6f:
		LY = A;
		continue;

	case 0x70:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7001;
			} else {
				m_ref = 0xfd7002;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7003;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7004;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7005;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7006;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7007;
			return;
		}
		//case 0x07:
		TDAT8 = B;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7008;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x71:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7101;
			} else {
				m_ref = 0xfd7102;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7103;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7104;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7105;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7106;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7107;
			return;
		}
		//case 0x07:
		TDAT8 = C;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7108;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x72:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7201;
			} else {
				m_ref = 0xfd7202;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7203;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7204;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7205;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7206;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7207;
			return;
		}
		//case 0x07:
		TDAT8 = D;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7208;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x73:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7301;
			} else {
				m_ref = 0xfd7302;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7303;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7304;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7305;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7306;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7307;
			return;
		}
		//case 0x07:
		TDAT8 = E;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7308;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x74:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7401;
			} else {
				m_ref = 0xfd7402;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7403;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7404;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7405;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7406;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7407;
			return;
		}
		//case 0x07:
		TDAT8 = H;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7408;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x75:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7501;
			} else {
				m_ref = 0xfd7502;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7503;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7504;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7505;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7506;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7507;
			return;
		}
		//case 0x07:
		TDAT8 = L;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7508;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x76:
		illegal_1();
		continue;

	case 0x77:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7701;
			} else {
				m_ref = 0xfd7702;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7703;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7704;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7705;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7706;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7707;
			return;
		}
		//case 0x07:
		TDAT8 = A;
		//case 0x08:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7708;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x78:
		illegal_1();
		continue;

	case 0x79:
		illegal_1();
		continue;

	case 0x7a:
		illegal_1();
		continue;

	case 0x7b:
		illegal_1();
		continue;

	case 0x7c:
		A = HY;
		continue;

	case 0x7d:
		A = LY;
		continue;

	case 0x7e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7e01;
			} else {
				m_ref = 0xfd7e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd7e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd7e08;
			} else {
				m_ref = 0xfd7e09;
			}
			return;
		}
		//case 0x09:
		A = TDAT8;
		continue;

	case 0x7f:
		illegal_1();
		continue;

	case 0x80:
		illegal_1();
		continue;

	case 0x81:
		illegal_1();
		continue;

	case 0x82:
		illegal_1();
		continue;

	case 0x83:
		illegal_1();
		continue;

	case 0x84:
		add_a(HY);
		continue;

	case 0x85:
		add_a(LY);
		continue;

	case 0x86:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd8601;
			} else {
				m_ref = 0xfd8602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd8603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd8604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd8605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd8606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd8607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd8608;
			} else {
				m_ref = 0xfd8609;
			}
			return;
		}
		//case 0x09:
		add_a(TDAT8);
		continue;

	case 0x87:
		illegal_1();
		continue;

	case 0x88:
		illegal_1();
		continue;

	case 0x89:
		illegal_1();
		continue;

	case 0x8a:
		illegal_1();
		continue;

	case 0x8b:
		illegal_1();
		continue;

	case 0x8c:
		adc_a(HY);
		continue;

	case 0x8d:
		adc_a(LY);
		continue;

	case 0x8e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd8e01;
			} else {
				m_ref = 0xfd8e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd8e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd8e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd8e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd8e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd8e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd8e08;
			} else {
				m_ref = 0xfd8e09;
			}
			return;
		}
		//case 0x09:
		adc_a(TDAT8);
		continue;

	case 0x8f:
		illegal_1();
		continue;

	case 0x90:
		illegal_1();
		continue;

	case 0x91:
		illegal_1();
		continue;

	case 0x92:
		illegal_1();
		continue;

	case 0x93:
		illegal_1();
		continue;

	case 0x94:
		sub_a(HY);
		continue;

	case 0x95:
		sub_a(LY);
		continue;

	case 0x96:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd9601;
			} else {
				m_ref = 0xfd9602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd9603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd9604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd9605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd9606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd9607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd9608;
			} else {
				m_ref = 0xfd9609;
			}
			return;
		}
		//case 0x09:
		sub_a(TDAT8);
		continue;

	case 0x97:
		illegal_1();
		continue;

	case 0x98:
		illegal_1();
		continue;

	case 0x99:
		illegal_1();
		continue;

	case 0x9a:
		illegal_1();
		continue;

	case 0x9b:
		illegal_1();
		continue;

	case 0x9c:
		sbc_a(HY);
		continue;

	case 0x9d:
		sbc_a(LY);
		continue;

	case 0x9e:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd9e01;
			} else {
				m_ref = 0xfd9e02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd9e03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd9e04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd9e05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd9e06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfd9e07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfd9e08;
			} else {
				m_ref = 0xfd9e09;
			}
			return;
		}
		//case 0x09:
		sbc_a(TDAT8);
		continue;

	case 0x9f:
		illegal_1();
		continue;

	case 0xa0:
		illegal_1();
		continue;

	case 0xa1:
		illegal_1();
		continue;

	case 0xa2:
		illegal_1();
		continue;

	case 0xa3:
		illegal_1();
		continue;

	case 0xa4:
		and_a(HY);
		continue;

	case 0xa5:
		and_a(LY);
		continue;

	case 0xa6:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfda601;
			} else {
				m_ref = 0xfda602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfda603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfda604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfda605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfda606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfda607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfda608;
			} else {
				m_ref = 0xfda609;
			}
			return;
		}
		//case 0x09:
		and_a(TDAT8);
		continue;

	case 0xa7:
		illegal_1();
		continue;

	case 0xa8:
		illegal_1();
		continue;

	case 0xa9:
		illegal_1();
		continue;

	case 0xaa:
		illegal_1();
		continue;

	case 0xab:
		illegal_1();
		continue;

	case 0xac:
		xor_a(HY);
		continue;

	case 0xad:
		xor_a(LY);
		continue;

	case 0xae:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfdae01;
			} else {
				m_ref = 0xfdae02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdae03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdae04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdae05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdae06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdae07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfdae08;
			} else {
				m_ref = 0xfdae09;
			}
			return;
		}
		//case 0x09:
		xor_a(TDAT8);
		continue;

	case 0xaf:
		illegal_1();
		continue;

	case 0xb0:
		illegal_1();
		continue;

	case 0xb1:
		illegal_1();
		continue;

	case 0xb2:
		illegal_1();
		continue;

	case 0xb3:
		illegal_1();
		continue;

	case 0xb4:
		or_a(HY);
		continue;

	case 0xb5:
		or_a(LY);
		continue;

	case 0xb6:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfdb601;
			} else {
				m_ref = 0xfdb602;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdb603;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdb604;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdb605;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdb606;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdb607;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfdb608;
			} else {
				m_ref = 0xfdb609;
			}
			return;
		}
		//case 0x09:
		or_a(TDAT8);
		continue;

	case 0xb7:
		illegal_1();
		continue;

	case 0xb8:
		illegal_1();
		continue;

	case 0xb9:
		illegal_1();
		continue;

	case 0xba:
		illegal_1();
		continue;

	case 0xbb:
		illegal_1();
		continue;

	case 0xbc:
		cp(HY);
		continue;

	case 0xbd:
		cp(LY);
		continue;

	case 0xbe:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfdbe01;
			} else {
				m_ref = 0xfdbe02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdbe03;
			return;
		}
		//case 0x03:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdbe04;
			return;
		}
		//case 0x04:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdbe05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdbe06;
			return;
		}
		//case 0x06:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdbe07;
			return;
		}
		//case 0x07:
		//case 0x08:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfdbe08;
			} else {
				m_ref = 0xfdbe09;
			}
			return;
		}
		//case 0x09:
		cp(TDAT8);
		continue;

	case 0xbf:
		illegal_1();
		continue;

	case 0xc0:
		illegal_1();
		continue;

	case 0xc1:
		illegal_1();
		continue;

	case 0xc2:
		illegal_1();
		continue;

	case 0xc3:
		illegal_1();
		continue;

	case 0xc4:
		illegal_1();
		continue;

	case 0xc5:
		illegal_1();
		continue;

	case 0xc6:
		illegal_1();
		continue;

	case 0xc7:
		illegal_1();
		continue;

	case 0xc8:
		illegal_1();
		continue;

	case 0xc9:
		illegal_1();
		continue;

	case 0xca:
		illegal_1();
		continue;

	case 0xcb:
		//case 0x00:
		//case 0x01:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfdcb01;
			} else {
				m_ref = 0xfdcb02;
			}
			return;
		}
		//case 0x02:
		PC++;
		m_ea = (u16)(IY + (s8)TDAT8);
		WZ = m_ea;
		//case 0x03:
		TDAT8 = arg_read();
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfdcb03;
			} else {
				m_ref = 0xfdcb04;
			}
			return;
		}
		//case 0x04:
		PC++;
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdcb05;
			return;
		}
		//case 0x05:
		if (nomreq_en)
			m_nomreq_cb(PC-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdcb06;
			return;
		}
		//case 0x06:
		m_ref = (0xfe << 16) | (TDAT8 << 8);
		goto process;
		continue;

	case 0xcc:
		illegal_1();
		continue;

	case 0xcd:
		illegal_1();
		continue;

	case 0xce:
		illegal_1();
		continue;

	case 0xcf:
		illegal_1();
		continue;

	case 0xd0:
		illegal_1();
		continue;

	case 0xd1:
		illegal_1();
		continue;

	case 0xd2:
		illegal_1();
		continue;

	case 0xd3:
		illegal_1();
		continue;

	case 0xd4:
		illegal_1();
		continue;

	case 0xd5:
		illegal_1();
		continue;

	case 0xd6:
		illegal_1();
		continue;

	case 0xd7:
		illegal_1();
		continue;

	case 0xd8:
		illegal_1();
		continue;

	case 0xd9:
		illegal_1();
		continue;

	case 0xda:
		illegal_1();
		continue;

	case 0xdb:
		illegal_1();
		continue;

	case 0xdc:
		illegal_1();
		continue;

	case 0xdd:
		illegal_1();
		continue;

	case 0xde:
		illegal_1();
		continue;

	case 0xdf:
		illegal_1();
		continue;

	case 0xe0:
		illegal_1();
		continue;

	case 0xe1:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfde101;
			} else {
				m_ref = 0xfde102;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfde103;
			} else {
				m_ref = 0xfde104;
			}
			return;
		}
		//case 0x04:
		SP++;
		IY = TDAT;
		continue;

	case 0xe2:
		illegal_1();
		continue;

	case 0xe3:
		//case 0x00:
		//case 0x01:
		TDAT_L = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfde301;
			} else {
				m_ref = 0xfde302;
			}
			return;
		}
		//case 0x02:
		SP++;
		//case 0x03:
		TDAT_H = stack_read(SP);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfde303;
			} else {
				m_ref = 0xfde304;
			}
			return;
		}
		//case 0x04:
		SP++;
		if (nomreq_en)
			m_nomreq_cb(SP-1, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfde305;
			return;
		}
		//case 0x05:
		SP--;
		//case 0x06:
		stack_write(SP, IY >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfde306;
			} else {
				m_ref = 0xfde307;
			}
			return;
		}
		//case 0x07:
		SP--;
		//case 0x08:
		stack_write(SP, IY);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfde308;
			} else {
				m_ref = 0xfde309;
			}
			return;
		}
		//case 0x09:
		IY = TDAT;
		if (nomreq_en)
			m_nomreq_cb(SP, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfde30a;
			return;
		}
		//case 0x0a:
		if (nomreq_en)
			m_nomreq_cb(SP, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfde30b;
			return;
		}
		//case 0x0b:
		WZ = TDAT;
		continue;

	case 0xe4:
		illegal_1();
		continue;

	case 0xe5:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfde501;
			return;
		}
		//case 0x01:
		SP--;
		//case 0x02:
		stack_write(SP, IY >> 8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfde502;
			} else {
				m_ref = 0xfde503;
			}
			return;
		}
		//case 0x03:
		SP--;
		//case 0x04:
		stack_write(SP, IY);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfde504;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xe6:
		illegal_1();
		continue;

	case 0xe7:
		illegal_1();
		continue;

	case 0xe8:
		illegal_1();
		continue;

	case 0xe9:
		PC = IY;
		continue;

	case 0xea:
		illegal_1();
		continue;

	case 0xeb:
		illegal_1();
		continue;

	case 0xec:
		illegal_1();
		continue;

	case 0xed:
		illegal_1();
		continue;

	case 0xee:
		illegal_1();
		continue;

	case 0xef:
		illegal_1();
		continue;

	case 0xf0:
		illegal_1();
		continue;

	case 0xf1:
		illegal_1();
		continue;

	case 0xf2:
		illegal_1();
		continue;

	case 0xf3:
		illegal_1();
		continue;

	case 0xf4:
		illegal_1();
		continue;

	case 0xf5:
		illegal_1();
		continue;

	case 0xf6:
		illegal_1();
		continue;

	case 0xf7:
		illegal_1();
		continue;

	case 0xf8:
		illegal_1();
		continue;

	case 0xf9:
		//case 0x00:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdf901;
			return;
		}
		//case 0x01:
		if (nomreq_en)
			m_nomreq_cb((m_i << 8) | (m_r2 & 0x80) | (m_r & 0x7f), 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfdf902;
			return;
		}
		//case 0x02:
		SP = IY;
		continue;

	case 0xfa:
		illegal_1();
		continue;

	case 0xfb:
		illegal_1();
		continue;

	case 0xfc:
		illegal_1();
		continue;

	case 0xfd:
		illegal_1();
		continue;

	case 0xfe:
		illegal_1();
		continue;

	case 0xff:
		illegal_1();
		continue;

	}
} break; // prefix: fd

case 0xfe:
{
	switch (u8(m_ref >> 8)) // opcode
	{
	case 0x00:
		illegal_1();
		continue;

	case 0x01:
		illegal_1();
		continue;

	case 0x02:
		illegal_1();
		continue;

	case 0x03:
		illegal_1();
		continue;

	case 0x04:
		illegal_1();
		continue;

	case 0x05:
		illegal_1();
		continue;

	case 0x06:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe0601;
			} else {
				m_ref = 0xfe0602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe0603;
			return;
		}
		//case 0x03:
		TDAT8 = rlc(TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe0604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x07:
		illegal_1();
		continue;

	case 0x08:
		illegal_1();
		continue;

	case 0x09:
		illegal_1();
		continue;

	case 0x0a:
		illegal_1();
		continue;

	case 0x0b:
		illegal_1();
		continue;

	case 0x0c:
		illegal_1();
		continue;

	case 0x0d:
		illegal_1();
		continue;

	case 0x0e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe0e01;
			} else {
				m_ref = 0xfe0e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe0e03;
			return;
		}
		//case 0x03:
		TDAT8 = rrc(TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe0e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x0f:
		illegal_1();
		continue;

	case 0x10:
		illegal_1();
		continue;

	case 0x11:
		illegal_1();
		continue;

	case 0x12:
		illegal_1();
		continue;

	case 0x13:
		illegal_1();
		continue;

	case 0x14:
		illegal_1();
		continue;

	case 0x15:
		illegal_1();
		continue;

	case 0x16:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe1601;
			} else {
				m_ref = 0xfe1602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe1603;
			return;
		}
		//case 0x03:
		TDAT8 = rl(TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe1604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x17:
		illegal_1();
		continue;

	case 0x18:
		illegal_1();
		continue;

	case 0x19:
		illegal_1();
		continue;

	case 0x1a:
		illegal_1();
		continue;

	case 0x1b:
		illegal_1();
		continue;

	case 0x1c:
		illegal_1();
		continue;

	case 0x1d:
		illegal_1();
		continue;

	case 0x1e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe1e01;
			} else {
				m_ref = 0xfe1e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe1e03;
			return;
		}
		//case 0x03:
		TDAT8 = rr(TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe1e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x1f:
		illegal_1();
		continue;

	case 0x20:
		illegal_1();
		continue;

	case 0x21:
		illegal_1();
		continue;

	case 0x22:
		illegal_1();
		continue;

	case 0x23:
		illegal_1();
		continue;

	case 0x24:
		illegal_1();
		continue;

	case 0x25:
		illegal_1();
		continue;

	case 0x26:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe2601;
			} else {
				m_ref = 0xfe2602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe2603;
			return;
		}
		//case 0x03:
		TDAT8 = sla(TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe2604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x27:
		illegal_1();
		continue;

	case 0x28:
		illegal_1();
		continue;

	case 0x29:
		illegal_1();
		continue;

	case 0x2a:
		illegal_1();
		continue;

	case 0x2b:
		illegal_1();
		continue;

	case 0x2c:
		illegal_1();
		continue;

	case 0x2d:
		illegal_1();
		continue;

	case 0x2e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe2e01;
			} else {
				m_ref = 0xfe2e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe2e03;
			return;
		}
		//case 0x03:
		TDAT8 = sra(TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe2e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x2f:
		illegal_1();
		continue;

	case 0x30:
		illegal_1();
		continue;

	case 0x31:
		illegal_1();
		continue;

	case 0x32:
		illegal_1();
		continue;

	case 0x33:
		illegal_1();
		continue;

	case 0x34:
		illegal_1();
		continue;

	case 0x35:
		illegal_1();
		continue;

	case 0x36:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe3601;
			} else {
				m_ref = 0xfe3602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe3603;
			return;
		}
		//case 0x03:
		TDAT8 = sll(TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe3604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x37:
		illegal_1();
		continue;

	case 0x38:
		illegal_1();
		continue;

	case 0x39:
		illegal_1();
		continue;

	case 0x3a:
		illegal_1();
		continue;

	case 0x3b:
		illegal_1();
		continue;

	case 0x3c:
		illegal_1();
		continue;

	case 0x3d:
		illegal_1();
		continue;

	case 0x3e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe3e01;
			} else {
				m_ref = 0xfe3e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe3e03;
			return;
		}
		//case 0x03:
		TDAT8 = srl(TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe3e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x3f:
		illegal_1();
		continue;

	case 0x40:
		illegal_1();
		continue;

	case 0x41:
		illegal_1();
		continue;

	case 0x42:
		illegal_1();
		continue;

	case 0x43:
		illegal_1();
		continue;

	case 0x44:
		illegal_1();
		continue;

	case 0x45:
		illegal_1();
		continue;

	case 0x46:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe4601;
			} else {
				m_ref = 0xfe4602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe4603;
			return;
		}
		//case 0x03:
		bit_xy(0, TDAT8);
		continue;

	case 0x47:
		illegal_1();
		continue;

	case 0x48:
		illegal_1();
		continue;

	case 0x49:
		illegal_1();
		continue;

	case 0x4a:
		illegal_1();
		continue;

	case 0x4b:
		illegal_1();
		continue;

	case 0x4c:
		illegal_1();
		continue;

	case 0x4d:
		illegal_1();
		continue;

	case 0x4e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe4e01;
			} else {
				m_ref = 0xfe4e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe4e03;
			return;
		}
		//case 0x03:
		bit_xy(1, TDAT8);
		continue;

	case 0x4f:
		illegal_1();
		continue;

	case 0x50:
		illegal_1();
		continue;

	case 0x51:
		illegal_1();
		continue;

	case 0x52:
		illegal_1();
		continue;

	case 0x53:
		illegal_1();
		continue;

	case 0x54:
		illegal_1();
		continue;

	case 0x55:
		illegal_1();
		continue;

	case 0x56:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe5601;
			} else {
				m_ref = 0xfe5602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe5603;
			return;
		}
		//case 0x03:
		bit_xy(2, TDAT8);
		continue;

	case 0x57:
		illegal_1();
		continue;

	case 0x58:
		illegal_1();
		continue;

	case 0x59:
		illegal_1();
		continue;

	case 0x5a:
		illegal_1();
		continue;

	case 0x5b:
		illegal_1();
		continue;

	case 0x5c:
		illegal_1();
		continue;

	case 0x5d:
		illegal_1();
		continue;

	case 0x5e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe5e01;
			} else {
				m_ref = 0xfe5e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe5e03;
			return;
		}
		//case 0x03:
		bit_xy(3, TDAT8);
		continue;

	case 0x5f:
		illegal_1();
		continue;

	case 0x60:
		illegal_1();
		continue;

	case 0x61:
		illegal_1();
		continue;

	case 0x62:
		illegal_1();
		continue;

	case 0x63:
		illegal_1();
		continue;

	case 0x64:
		illegal_1();
		continue;

	case 0x65:
		illegal_1();
		continue;

	case 0x66:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe6601;
			} else {
				m_ref = 0xfe6602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe6603;
			return;
		}
		//case 0x03:
		bit_xy(4, TDAT8);
		continue;

	case 0x67:
		illegal_1();
		continue;

	case 0x68:
		illegal_1();
		continue;

	case 0x69:
		illegal_1();
		continue;

	case 0x6a:
		illegal_1();
		continue;

	case 0x6b:
		illegal_1();
		continue;

	case 0x6c:
		illegal_1();
		continue;

	case 0x6d:
		illegal_1();
		continue;

	case 0x6e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe6e01;
			} else {
				m_ref = 0xfe6e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe6e03;
			return;
		}
		//case 0x03:
		bit_xy(5, TDAT8);
		continue;

	case 0x6f:
		illegal_1();
		continue;

	case 0x70:
		illegal_1();
		continue;

	case 0x71:
		illegal_1();
		continue;

	case 0x72:
		illegal_1();
		continue;

	case 0x73:
		illegal_1();
		continue;

	case 0x74:
		illegal_1();
		continue;

	case 0x75:
		illegal_1();
		continue;

	case 0x76:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe7601;
			} else {
				m_ref = 0xfe7602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe7603;
			return;
		}
		//case 0x03:
		bit_xy(6, TDAT8);
		continue;

	case 0x77:
		illegal_1();
		continue;

	case 0x78:
		illegal_1();
		continue;

	case 0x79:
		illegal_1();
		continue;

	case 0x7a:
		illegal_1();
		continue;

	case 0x7b:
		illegal_1();
		continue;

	case 0x7c:
		illegal_1();
		continue;

	case 0x7d:
		illegal_1();
		continue;

	case 0x7e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe7e01;
			} else {
				m_ref = 0xfe7e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe7e03;
			return;
		}
		//case 0x03:
		bit_xy(7, TDAT8);
		continue;

	case 0x7f:
		illegal_1();
		continue;

	case 0x80:
		illegal_1();
		continue;

	case 0x81:
		illegal_1();
		continue;

	case 0x82:
		illegal_1();
		continue;

	case 0x83:
		illegal_1();
		continue;

	case 0x84:
		illegal_1();
		continue;

	case 0x85:
		illegal_1();
		continue;

	case 0x86:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe8601;
			} else {
				m_ref = 0xfe8602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe8603;
			return;
		}
		//case 0x03:
		TDAT8 = res(0, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe8604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x87:
		illegal_1();
		continue;

	case 0x88:
		illegal_1();
		continue;

	case 0x89:
		illegal_1();
		continue;

	case 0x8a:
		illegal_1();
		continue;

	case 0x8b:
		illegal_1();
		continue;

	case 0x8c:
		illegal_1();
		continue;

	case 0x8d:
		illegal_1();
		continue;

	case 0x8e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe8e01;
			} else {
				m_ref = 0xfe8e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe8e03;
			return;
		}
		//case 0x03:
		TDAT8 = res(1, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe8e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x8f:
		illegal_1();
		continue;

	case 0x90:
		illegal_1();
		continue;

	case 0x91:
		illegal_1();
		continue;

	case 0x92:
		illegal_1();
		continue;

	case 0x93:
		illegal_1();
		continue;

	case 0x94:
		illegal_1();
		continue;

	case 0x95:
		illegal_1();
		continue;

	case 0x96:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe9601;
			} else {
				m_ref = 0xfe9602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe9603;
			return;
		}
		//case 0x03:
		TDAT8 = res(2, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe9604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x97:
		illegal_1();
		continue;

	case 0x98:
		illegal_1();
		continue;

	case 0x99:
		illegal_1();
		continue;

	case 0x9a:
		illegal_1();
		continue;

	case 0x9b:
		illegal_1();
		continue;

	case 0x9c:
		illegal_1();
		continue;

	case 0x9d:
		illegal_1();
		continue;

	case 0x9e:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe9e01;
			} else {
				m_ref = 0xfe9e02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfe9e03;
			return;
		}
		//case 0x03:
		TDAT8 = res(3, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfe9e04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0x9f:
		illegal_1();
		continue;

	case 0xa0:
		illegal_1();
		continue;

	case 0xa1:
		illegal_1();
		continue;

	case 0xa2:
		illegal_1();
		continue;

	case 0xa3:
		illegal_1();
		continue;

	case 0xa4:
		illegal_1();
		continue;

	case 0xa5:
		illegal_1();
		continue;

	case 0xa6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfea601;
			} else {
				m_ref = 0xfea602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfea603;
			return;
		}
		//case 0x03:
		TDAT8 = res(4, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfea604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xa7:
		illegal_1();
		continue;

	case 0xa8:
		illegal_1();
		continue;

	case 0xa9:
		illegal_1();
		continue;

	case 0xaa:
		illegal_1();
		continue;

	case 0xab:
		illegal_1();
		continue;

	case 0xac:
		illegal_1();
		continue;

	case 0xad:
		illegal_1();
		continue;

	case 0xae:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfeae01;
			} else {
				m_ref = 0xfeae02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfeae03;
			return;
		}
		//case 0x03:
		TDAT8 = res(5, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfeae04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xaf:
		illegal_1();
		continue;

	case 0xb0:
		illegal_1();
		continue;

	case 0xb1:
		illegal_1();
		continue;

	case 0xb2:
		illegal_1();
		continue;

	case 0xb3:
		illegal_1();
		continue;

	case 0xb4:
		illegal_1();
		continue;

	case 0xb5:
		illegal_1();
		continue;

	case 0xb6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfeb601;
			} else {
				m_ref = 0xfeb602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfeb603;
			return;
		}
		//case 0x03:
		TDAT8 = res(6, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfeb604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xb7:
		illegal_1();
		continue;

	case 0xb8:
		illegal_1();
		continue;

	case 0xb9:
		illegal_1();
		continue;

	case 0xba:
		illegal_1();
		continue;

	case 0xbb:
		illegal_1();
		continue;

	case 0xbc:
		illegal_1();
		continue;

	case 0xbd:
		illegal_1();
		continue;

	case 0xbe:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfebe01;
			} else {
				m_ref = 0xfebe02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfebe03;
			return;
		}
		//case 0x03:
		TDAT8 = res(7, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfebe04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xbf:
		illegal_1();
		continue;

	case 0xc0:
		illegal_1();
		continue;

	case 0xc1:
		illegal_1();
		continue;

	case 0xc2:
		illegal_1();
		continue;

	case 0xc3:
		illegal_1();
		continue;

	case 0xc4:
		illegal_1();
		continue;

	case 0xc5:
		illegal_1();
		continue;

	case 0xc6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfec601;
			} else {
				m_ref = 0xfec602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfec603;
			return;
		}
		//case 0x03:
		TDAT8 = set(0, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfec604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xc7:
		illegal_1();
		continue;

	case 0xc8:
		illegal_1();
		continue;

	case 0xc9:
		illegal_1();
		continue;

	case 0xca:
		illegal_1();
		continue;

	case 0xcb:
		illegal_1();
		continue;

	case 0xcc:
		illegal_1();
		continue;

	case 0xcd:
		illegal_1();
		continue;

	case 0xce:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfece01;
			} else {
				m_ref = 0xfece02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfece03;
			return;
		}
		//case 0x03:
		TDAT8 = set(1, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfece04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xcf:
		illegal_1();
		continue;

	case 0xd0:
		illegal_1();
		continue;

	case 0xd1:
		illegal_1();
		continue;

	case 0xd2:
		illegal_1();
		continue;

	case 0xd3:
		illegal_1();
		continue;

	case 0xd4:
		illegal_1();
		continue;

	case 0xd5:
		illegal_1();
		continue;

	case 0xd6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfed601;
			} else {
				m_ref = 0xfed602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfed603;
			return;
		}
		//case 0x03:
		TDAT8 = set(2, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfed604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xd7:
		illegal_1();
		continue;

	case 0xd8:
		illegal_1();
		continue;

	case 0xd9:
		illegal_1();
		continue;

	case 0xda:
		illegal_1();
		continue;

	case 0xdb:
		illegal_1();
		continue;

	case 0xdc:
		illegal_1();
		continue;

	case 0xdd:
		illegal_1();
		continue;

	case 0xde:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfede01;
			} else {
				m_ref = 0xfede02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfede03;
			return;
		}
		//case 0x03:
		TDAT8 = set(3, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfede04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xdf:
		illegal_1();
		continue;

	case 0xe0:
		illegal_1();
		continue;

	case 0xe1:
		illegal_1();
		continue;

	case 0xe2:
		illegal_1();
		continue;

	case 0xe3:
		illegal_1();
		continue;

	case 0xe4:
		illegal_1();
		continue;

	case 0xe5:
		illegal_1();
		continue;

	case 0xe6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfee601;
			} else {
				m_ref = 0xfee602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfee603;
			return;
		}
		//case 0x03:
		TDAT8 = set(4, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfee604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xe7:
		illegal_1();
		continue;

	case 0xe8:
		illegal_1();
		continue;

	case 0xe9:
		illegal_1();
		continue;

	case 0xea:
		illegal_1();
		continue;

	case 0xeb:
		illegal_1();
		continue;

	case 0xec:
		illegal_1();
		continue;

	case 0xed:
		illegal_1();
		continue;

	case 0xee:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfeee01;
			} else {
				m_ref = 0xfeee02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfeee03;
			return;
		}
		//case 0x03:
		TDAT8 = set(5, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfeee04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xef:
		illegal_1();
		continue;

	case 0xf0:
		illegal_1();
		continue;

	case 0xf1:
		illegal_1();
		continue;

	case 0xf2:
		illegal_1();
		continue;

	case 0xf3:
		illegal_1();
		continue;

	case 0xf4:
		illegal_1();
		continue;

	case 0xf5:
		illegal_1();
		continue;

	case 0xf6:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfef601;
			} else {
				m_ref = 0xfef602;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfef603;
			return;
		}
		//case 0x03:
		TDAT8 = set(6, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfef604;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xf7:
		illegal_1();
		continue;

	case 0xf8:
		illegal_1();
		continue;

	case 0xf9:
		illegal_1();
		continue;

	case 0xfa:
		illegal_1();
		continue;

	case 0xfb:
		illegal_1();
		continue;

	case 0xfc:
		illegal_1();
		continue;

	case 0xfd:
		illegal_1();
		continue;

	case 0xfe:
		//case 0x00:
		//case 0x01:
		TDAT8 = data_read(m_ea);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfefe01;
			} else {
				m_ref = 0xfefe02;
			}
			return;
		}
		//case 0x02:
		if (nomreq_en)
			m_nomreq_cb(m_ea, 0x00, 0xff);
		m_icount -= 1;
		if (m_icount <= 0) {
			m_ref = 0xfefe03;
			return;
		}
		//case 0x03:
		TDAT8 = set(7, TDAT8);
		//case 0x04:
		data_write(m_ea, TDAT8);
		m_icount -= m_mreq_cycles;
		if (m_icount <= 0) {
			if (access_to_be_redone()) {
				m_icount += m_mreq_cycles;
				m_ref = 0xfefe04;
			} else {
				m_ref = 0xffff00;
			}
			return;
		}
		continue;

	case 0xff:
		illegal_1();
		continue;

	}
} break; // prefix: fe

} // switch prefix
} // end: fast
assert((void("unreachable!"), false));
