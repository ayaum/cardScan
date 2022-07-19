import { setDoc, doc, arrayUnion, updateDoc, getDoc } from 'firebase/firestore'
import dayjs from 'dayjs'
import db from '../plugins/firebase'
const express = require('express')
const bodyParser = require('body-parser')
const app = express()
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())

app.get('/', (req, res) => {
  res.json({ message: 'hello, api' })
})
module.exports = {
  path: '/api',
  handler: app
}

app.post('/save', function (request, response) {
  const day = dayjs().format('YYYYMMDD')
  const docRef = doc(db, 'scanTime', day)
  const saveData = {
    time: dayjs().format('HH:mm:ss'),
    id: request.body.id
  }
  getDoc(docRef).then((data) => {
    if (data.exists()) {
      updateData(day, saveData)
    } else {
      setData(day, saveData)
    }
  })

  response.send('Received POST Data!')
})

const updateData = (day, saveData) => {
  updateDoc(doc(db, 'scanTime', day), {
    scanTimes: arrayUnion(saveData)
  })
}

const setData = (day, saveData) => {
  setDoc(doc(db, 'scanTime', day), {
    scanTimes: arrayUnion(saveData)
  })
}
