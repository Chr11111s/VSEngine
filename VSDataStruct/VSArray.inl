template <class T,class VSMemManagerClass>
VSArray<T, VSMemManagerClass>::~VSArray()
{
	Delete(m_pBuffer,m_uiCurUse);
	m_uiBufferNum = 0;
	m_uiCurUse = 0;
	m_uiAllocNum = 0;
}

template <class T,class VSMemManagerClass>
VSArray<T, VSMemManagerClass>::VSArray(unsigned int uiGrowBy)
{
	VSMAC_ASSERT(uiGrowBy);
	m_uiGrowBy = uiGrowBy;
	m_pBuffer = NULL;
	m_uiCurUse = 0;
	m_uiBufferNum = 0;
	m_uiAllocNum = 0;
}
template <class T,class VSMemManagerClass>
VSArray<T, VSMemManagerClass>::VSArray(const VSArray<T, VSMemManagerClass>& Array)
{
	m_uiGrowBy = Array.m_uiGrowBy;
	m_pBuffer = NULL;
	m_uiCurUse = 0;
	m_uiBufferNum = 0;
	m_uiAllocNum = 0;
	*this = Array;
}
template <class T, class VSMemManagerClass>
void VSArray<T, VSMemManagerClass>::GetNoHaveElement(const VSArray<T, VSMemManagerClass>& Array)
{
	for (unsigned int i = 0; i < Array.GetNum(); i++)
	{	
		unsigned int Index = 0;
		for (; Index < GetNum(); Index++)
		{
			if (m_pBuffer[Index] == Array[i])
			{
				break;
			}
		}
		if (Index == GetNum())
		{
			AddElement(Array[i]);
		}		
	}
}
template <class T,class VSMemManagerClass>
void VSArray<T, VSMemManagerClass>::SetBufferNum(unsigned int uiBufferNum)
{
	if(uiBufferNum)
	{
		Delete(m_pBuffer,m_uiCurUse);
		m_uiBufferNum = uiBufferNum;	

		m_pBuffer = New(uiBufferNum);
		VSMAC_ASSERT(m_pBuffer);
		m_uiCurUse = uiBufferNum;
		for (unsigned int i = 0 ; i  < m_uiBufferNum ; i++)
		{
			VS_NEW (m_pBuffer + i) T();

		}
	}	

}
template <class T,class VSMemManagerClass>
void VSArray<T, VSMemManagerClass>::AddBufferNum(unsigned int uiBufferNum)
{
	if(uiBufferNum)
	{
		//���������Ҫ�����ڴ棬�Ͱ�m_uiGrowBy������
		m_uiAllocNum++;
		m_uiGrowBy = m_uiAllocNum * m_uiGrowBy;
	}
	T * pBuffer = NULL;

	pBuffer = New(m_uiBufferNum + uiBufferNum);
	VSMAC_ASSERT(pBuffer);
	if (m_pBuffer && m_uiCurUse)
	{
		for (unsigned int i = 0; i < m_uiCurUse; i++)
		{
			VS_NEW(pBuffer + i) T(m_pBuffer[i]);

		}

	}
	Delete(m_pBuffer, m_uiCurUse);
	m_uiBufferNum = m_uiBufferNum + uiBufferNum;
	m_pBuffer = pBuffer;
}
template <class T,class VSMemManagerClass>
inline unsigned int VSArray<T, VSMemManagerClass>::GetNum()const
{
	return m_uiCurUse;
}
template <class T,class VSMemManagerClass>	
inline unsigned int VSArray<T, VSMemManagerClass>::GetBufferNum()const
{
	return m_uiBufferNum;
}
template <class T,class VSMemManagerClass>
inline T * VSArray<T, VSMemManagerClass>::GetBuffer()const
{
	return m_pBuffer;
}
template <class T,class VSMemManagerClass>
inline void VSArray<T, VSMemManagerClass>::SetGrowBy(unsigned int uiGrowBy)
{
	m_uiGrowBy = uiGrowBy;
}
template <class T,class VSMemManagerClass>
inline unsigned int VSArray<T, VSMemManagerClass>::GetSize()const
{
	return sizeof(unsigned int ) * 4 + sizeof(T *) + sizeof(T) * m_uiBufferNum;
}
template <class T,class VSMemManagerClass>
unsigned int VSArray<T, VSMemManagerClass>::AddElement(const T & Element)
{
	if(m_uiCurUse == m_uiBufferNum)
	{
		AddBufferNum(m_uiGrowBy);
	}
	VS_NEW (m_pBuffer + m_uiCurUse) T(Element);
	m_uiCurUse++;
	return m_uiCurUse - 1;
}
template <class T,class VSMemManagerClass>
template <class N, class VSMemManagerClassN>
void VSArray<T, VSMemManagerClass>::AddElement(const VSArray<N, VSMemManagerClassN> & Array,unsigned int uiBegin,unsigned int uiEnd)
{
	VSMAC_ASSERT(uiBegin <= uiEnd);
	VSMAC_ASSERT(uiEnd < Array.GetNum());


	int iAddNum = uiEnd - uiBegin + 1;
	int iHaveNum = m_uiBufferNum - m_uiCurUse;

	int iNeedNum = iHaveNum - iAddNum;

	if (iNeedNum < 0)
	{
		if(m_uiGrowBy)
		{
			iNeedNum = -iNeedNum;
			if( (unsigned int)iNeedNum < m_uiGrowBy)
				AddBufferNum(m_uiGrowBy);
			else
			{
				AddBufferNum(iNeedNum);
			}
		}
		else
		{
			iAddNum = iHaveNum;
		}
	}

	if(iAddNum == 1)
	{
		AddElement(Array[uiBegin]);
	}	
	else if(iAddNum == 2)
	{
		AddElement(Array[uiBegin]);
		AddElement(Array[uiBegin + 1]);
	}
	else
	{
		N * pBuffer = Array.GetBuffer();
		for (unsigned int i = m_uiCurUse ;i < (unsigned int)iAddNum + m_uiCurUse ; i++)
		{
			VS_NEW (m_pBuffer + i) T(pBuffer[uiBegin]);
			uiBegin++;
		}
		m_uiCurUse += iAddNum;
		//VSMemcpy(&m_pBuffer[m_uiCurUse],&Array.m_pBuffer[uiBegin], sizeof(T) * iAddNum);
	}
	
}
template <class T,class VSMemManagerClass>
void VSArray<T, VSMemManagerClass>::operator= (const VSArray<T, VSMemManagerClass>& Array)
{
	if (m_uiBufferNum >= Array.GetNum())
	{
		Clear();
		m_uiCurUse = Array.GetNum();
		T * pBuffer = Array.GetBuffer();
		for (unsigned int i = 0 ;i < m_uiCurUse ; i++)
		{
			VS_NEW (m_pBuffer + i) T(pBuffer[i]);
		}
	}
	else
	{
		Delete(m_pBuffer,m_uiCurUse);
		m_uiBufferNum = Array.GetBufferNum();

		m_uiCurUse = Array.GetNum();

		m_pBuffer = New(m_uiBufferNum);
		if(!m_pBuffer)
			return ;
		T * pBuffer = Array.GetBuffer();
		for (unsigned int i = 0 ;i < m_uiCurUse ; i++)
		{
			VS_NEW (m_pBuffer + i) T(pBuffer[i]);
		}


	}
	
	return ;
}
template <class T,class VSMemManagerClass>
void VSArray<T, VSMemManagerClass>::Clear()
{
	if (m_uiCurUse == 0)
	{
		return ;
	}
	if (ValueBase<T>::NeedsDestructor)
	{
		for (unsigned int i =0 ; i < m_uiCurUse ; i++)
		{
			m_pBuffer[i].~T();
		}
	}

	m_uiCurUse = 0;

}
template <class T,class VSMemManagerClass>
void VSArray<T, VSMemManagerClass>::Destroy()
{
	Clear();
	m_uiBufferNum = 0;
	Delete(m_pBuffer,m_uiCurUse);
}
template <class T,class VSMemManagerClass>
T& VSArray<T, VSMemManagerClass>::operator[] (unsigned int i)const
{

	VSMAC_ASSERT(i < m_uiBufferNum);	
	return m_pBuffer[i];

	
}
template <class T,class VSMemManagerClass>
void VSArray<T, VSMemManagerClass>::Erase(unsigned int i)
{

	VSMAC_ASSERT(i < m_uiCurUse)
	{
		Erase(i, i);
	}	
}
template <class T,class VSMemManagerClass>
void VSArray<T, VSMemManagerClass>::Erase(unsigned int uiBegin,unsigned int uiEnd)
{
	VSMAC_ASSERT(uiEnd < m_uiCurUse);

	VSMAC_ASSERT(uiBegin <= uiEnd);

	unsigned int k;
	unsigned int uiMoveNum = m_uiCurUse -1 - uiEnd;
	unsigned int uiMoveIndex ;
	if (ValueBase<T>::NeedsDestructor)
	{
		for (unsigned int uiDelete = uiBegin ; uiDelete <= uiEnd ; uiDelete++)
		{
			m_pBuffer[uiDelete].~T();
		}
	}
	for (k = 0 ; k < uiMoveNum ; k++)
	{
		uiMoveIndex = uiEnd + k + 1;
		if (uiMoveIndex < m_uiCurUse)
		{
			VS_NEW (m_pBuffer + uiBegin + k) T(m_pBuffer[uiMoveIndex]);
			if (ValueBase<T>::NeedsDestructor)
			{
				m_pBuffer[uiMoveIndex].~T();
			}
		}
		
		
	}
	m_uiCurUse -= ((uiEnd - uiBegin) + 1) ;
}
template <class T, class VSMemManagerClass>
void VSArray<T, VSMemManagerClass>::SortAll()
{
	if (!m_uiCurUse)
	{
		return;
	}
	Sort(0, m_uiCurUse - 1);
}
template <class T,class VSMemManagerClass>
void VSArray<T, VSMemManagerClass>::Sort(unsigned int uiBegin,unsigned int uiEnd)
{
	VSMAC_ASSERT(uiEnd < m_uiCurUse);

	VSMAC_ASSERT(uiBegin <= uiEnd);
	//�鲢���� ���Ӷ� nlogn
	T * pBuffer = New(uiEnd - uiBegin + 1);
	for (unsigned int i = 0; i < uiEnd - uiBegin + 1; i++)
	{
		VS_NEW(pBuffer + i) T();
	}
	MergeSort(m_pBuffer + uiBegin, pBuffer, uiEnd - uiBegin + 1);
	Delete(pBuffer, uiEnd - uiBegin + 1);
//	ð������ ���Ӷ� n(n+1)/2
// 	for (unsigned int i = uiBegin ; i < uiEnd; i++)
// 	{
// 		for (unsigned int j = uiBegin ; j < uiEnd - uiBegin - i; j++)
// 		{
// 			if(m_pBuffer[j] > m_pBuffer[j + 1])
// 			{
// 				Swap(m_pBuffer[j],m_pBuffer[j + 1]);
// 			}
// 		}
// 	}

}
template <class T, class VSMemManagerClass>
template <class N>
void VSArray<T, VSMemManagerClass>::SortAll(N & Compare)
{
	if (!m_uiCurUse)
	{
		return;
	}
	Sort(0, m_uiCurUse, Compare);
}
template <class T,class VSMemManagerClass>
template <class N>
void VSArray<T, VSMemManagerClass>::Sort(unsigned int uiBegin,unsigned int uiEnd,N& Compare)
{
	if (uiEnd >= m_uiCurUse)
	{
		uiEnd = m_uiCurUse - 1;
	}
	if (uiBegin >= uiEnd)
	{
		return ;
	}
//�鲢���� ���Ӷ� nlogn
	T * pBuffer = New(uiEnd - uiBegin + 1);
	for (unsigned int i = 0; i < uiEnd - uiBegin + 1;i++)
	{
		VS_NEW(pBuffer + i) T();
	}
	MergeSort(m_pBuffer + uiBegin, pBuffer, uiEnd - uiBegin + 1, Compare);
	Delete(pBuffer, uiEnd - uiBegin + 1);
//	ð������ ���Ӷ� n(n+1)/2
// 	for (unsigned int i = uiBegin ; i < uiEnd; i++)
// 	{
// 		for (unsigned int j = uiBegin ; j < uiEnd - uiBegin - i; j++)
// 		{
// 			if(Compare(m_pBuffer[j],m_pBuffer[j + 1]) == 0)
// 			{
// 				Swap(m_pBuffer[j],m_pBuffer[j + 1]);
// 			}
// 		}
// 	}

}
template <class T,class VSMemManagerClass>
unsigned int VSArray<T, VSMemManagerClass>::FindElement(const T & Element)
{
	for (unsigned int i = 0 ;i < m_uiCurUse ; i++)
	{
		if (Element == m_pBuffer[i])
		{
			return i;
		}
	}
	return m_uiCurUse;
}
template <class T,class VSMemManagerClass>
VSArrayOrder<T, VSMemManagerClass>::VSArrayOrder(unsigned int uiGrowBy)
:VSArray(uiGrowBy)
{
}
template <class T,class VSMemManagerClass>
VSArrayOrder<T, VSMemManagerClass>::~VSArrayOrder()
{

}
template <class T,class VSMemManagerClass>
unsigned int VSArrayOrder<T, VSMemManagerClass>::AddElement(const T & Element)
{
	if(m_uiCurUse == m_uiBufferNum)
	{
		if(!m_uiGrowBy)
			return -1;
		AddBufferNum(m_uiGrowBy);
	}

	unsigned int uiIndex;
	if (m_uiCurUse == 0)
	{
		uiIndex = 0;	
	}
	else if (m_uiCurUse == 1)
	{
		if (m_pBuffer[0] > Element)
		{
			uiIndex = 0;
		}
		else
		{
			uiIndex = 1;
		}
	}
	else if (m_pBuffer[0] > Element)
	{
		uiIndex = 0;
	}
	else if (Element > m_pBuffer[m_uiCurUse - 1])
	{
		uiIndex = m_uiCurUse;
	}
	else
	{
		uiIndex = Process(0,m_uiCurUse - 1,Element);
	}
	
	if (m_uiCurUse == uiIndex)
	{
		VS_NEW (m_pBuffer + uiIndex) T(Element);

	}
	else
	{
		VS_NEW (m_pBuffer + m_uiCurUse) T(m_pBuffer[m_uiCurUse - 1]);
		for (int i = (int)m_uiCurUse - 2 ; i >= (int)uiIndex  ;i--)
		{
			m_pBuffer[i + 1] = m_pBuffer[i];

		}
		m_pBuffer[uiIndex] = Element;
	}

	m_uiCurUse++;
	return uiIndex;
}
template <class T,class VSMemManagerClass>
unsigned int VSArrayOrder<T, VSMemManagerClass>::FindElement(const T & Element)
{
	if (m_uiCurUse == 0)
	{
		return 0;
	}
	if (m_pBuffer[0] > Element)
	{
		return m_uiCurUse;
	}
	else if (Element > m_pBuffer[m_uiCurUse - 1])
	{ 
		return m_uiCurUse;
	}
	else
	{
		return FindElement(0,m_uiCurUse - 1,Element);
	}
}	
template <class T,class VSMemManagerClass>
void VSArrayOrder<T, VSMemManagerClass>::ReSort(unsigned int i)
{
	VSMAC_ASSERT(i < m_uiCurUse);
	if (m_pBuffer[i] < m_pBuffer[i - 1] && i)
	{
		T Temp = m_pBuffer[i];
		unsigned int uiIndex = Process(0,i - 1,m_pBuffer[i]);
		for (int k = i - 1 ; k >= (int)uiIndex  ; k--)
		{
			m_pBuffer[k + 1] = m_pBuffer[k];
		}
		m_pBuffer[uiIndex] = Temp;
	}
	else if (m_pBuffer[i] > m_pBuffer[i + 1] && i != m_uiCurUse - 1)
	{
		unsigned int uiIndex = Process(i + 1,m_uiCurUse - 1,m_pBuffer[i]);
		T Temp = m_pBuffer[i];
		for (unsigned int k = i ; k <= uiIndex - 1 ; k++)
		{
			m_pBuffer[k] = m_pBuffer[k + 1];
		}
		m_pBuffer[uiIndex] = Temp;
	}
}
template <class T,class VSMemManagerClass>
unsigned int VSArrayOrder<T, VSMemManagerClass>::FindElement(unsigned int uiIndex0,unsigned int uiIndex1,const T & Element)
{
	if (uiIndex0 == uiIndex1)
	{
		if (m_pBuffer[uiIndex0] == Element)
		{
			return uiIndex0;
		}
		else
		{
			return m_uiCurUse;
		}
	}
	else if (uiIndex1 - uiIndex0 == 1)
	{
		if (m_pBuffer[uiIndex0] == Element)
		{
			return uiIndex0;
		}
		else if (m_pBuffer[uiIndex1] == Element)
		{
			return uiIndex1;
		}
		else
		{
			return m_uiCurUse;
		}
	}
	else
	{
		unsigned int uiIndex = (uiIndex0 + uiIndex1) >> 1;
		if (m_pBuffer[uiIndex] == Element)
		{
			return uiIndex;
		}
		else if(m_pBuffer[uiIndex] > Element)
		{
			return FindElement(uiIndex0,uiIndex,Element);
		}
		else
		{
			return FindElement(uiIndex,uiIndex1,Element);
		}

	}
}
template <class T,class VSMemManagerClass>
unsigned int VSArrayOrder<T, VSMemManagerClass>::Process(unsigned int uiIndex0,unsigned int uiIndex1,const T & Element)
{
	if (uiIndex0 == uiIndex1)
	{
		return uiIndex0;
	}
	else if (uiIndex1 - uiIndex0 == 1)
	{
		if (Element < m_pBuffer[uiIndex0])
		{
			return uiIndex0;
		}
		else
		{
			return uiIndex1;
		}
		
	}
	else
	{
		unsigned int uiIndex = (uiIndex0 + uiIndex1) >> 1;
		if (m_pBuffer[uiIndex] == Element)
		{
			return uiIndex;
		}
		else if(m_pBuffer[uiIndex] > Element)
		{
			return Process(uiIndex0,uiIndex,Element);
		}
		else
		{
			return Process(uiIndex,uiIndex1,Element);
		}

	}
}